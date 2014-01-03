#include "variablemodelmanager.h"

VariableModelManager::VariableModelManager(QObject *parent) : QObject(parent)
{

}

/* Local variable */
/* ********************************************************************************************************* */

/*!
 * \brief VariableModelManager::localVariable : Get a local variable or initialize it with the default value
 * \param name
 * \param defaultValue
 * \return SystemVariable*
 */

SystemVariable* VariableModelManager::localVariable(QString name, QVariant defaultValue)
{
    if (!m_localList.contains(name)){

        m_localList.insert(name, new SystemVariable(name, defaultValue, "",this));
    }

    SystemVariable* str = m_localList.value(name);

    return str;
}

/*!
 * \brief VariableModelManager::setLocalVariable : Initialize or modify a local variable
 * \param name
 * \param value
 */

void VariableModelManager::setLocalVariable(QString name, QVariant value)
{

    if (!m_localList.contains(name)){

        m_localList.insert(name, new SystemVariable(name, value, "",this));
    }
    else{

        SystemVariable* str = m_localList.value(name);
        str->setVariable(value);
    }
}

/* Server variable */
/* ********************************************************************************************************* */

/*!
 * \brief VariableModelManager::requestSystemVariable : Request the variable to the server
 * \param name
 * \param filter
 * \return SystemVariable*
 */

SystemVariable* VariableModelManager::requestSystemVariable(QString name, QString filter)
{
    // Should I initialize the variable

    if (!m_list.contains(name)){

        m_list.insert(name, new SystemVariable(name, "", filter,this));

        if (m_serverAvailable)
            emit requestVariable(name,filter);
    }

    // Getting current variable registered

    SystemVariable* str = m_list.value(name);

    // Checking if the filter has changed

    if (str->option() != filter){

        str->setOption(filter);
        str->setVariable("");

        if (m_serverAvailable)
            emit requestVariable(name,filter);
    }

    // Returning the variable

    return str;
}

/*!
 * \brief VariableModelManager::systemVariable : Get a system variable requested by an other function
 * \param name
 * \param filter
 * \return SystemVariable*
 */

SystemVariable* VariableModelManager::systemVariable(QString name, QString filter)
{
    // Should I initialize the variable

    if (!m_list.contains(name)){

        m_list.insert(name, new SystemVariable(name, "", filter,this));
    }

    // Getting current variable registered

    SystemVariable* str = m_list.value(name);

    // Checking if the filter has changed

    if (str->option() != filter){

        str->setOption(filter);
        str->setVariable("");
    }

    // Returning the variable

    return str;
}

/*!
 * \brief VariableModelManager::releaseSystemVariable : Release a system variable
 * \param name of the variable
 */

void VariableModelManager::releaseSystemVariable(QString name)
{
    if (m_list.contains(name)){

        if (m_serverAvailable)
            emit releaseVariable(name);

        SystemVariable * model = m_list.value(name);
        model->deleteLater();
        m_list.remove(name);
    }
}

/*!
 * \brief VariableModelManager::updateSystemVariable : Slot to modify an internal variable
 * \param name
 * \param filter
 * \param varContent
 */

void VariableModelManager::updateSystemVariable(QString name, QString filter, QVariant varContent)
{
    if (m_list.contains(name)){

        SystemVariable* str = m_list.value(name);

        if (str->option() == filter){

            str->setVariable(varContent);
        }
        else{

            log("Filters doesn't match the request, aborting");        }
    }
    else{

        log("WARNING : Creating variable from server : " + name);

        m_list.insert(name, new SystemVariable(name, varContent, filter,this));
    }
}

/*!
 * \brief VariableModelManager::setServerAvailable : Modify the status of the server
 * \param server availability
 */

void VariableModelManager::setServerAvailable(bool available)
{
    m_serverAvailable = available;
    emit serverAvailableChanged(available);

    if (available){

        updateModel();
    }
}

/*!
 * \brief VariableModelManager::updateModel : Update the entire model
 */

void VariableModelManager::updateModel()
{

    foreach (SystemVariable *var, m_list) {

        emit requestVariable(var->name(),var->option());
    }
}
