#ifndef JSONMODEL_H
#define JSONMODEL_H

#include <QObject>
#include <QString>
#include <QHash>

#include "../Log/log.h"

#include "systemvariable.h"

class VariableModelManager : public QObject
{
    Q_OBJECT

    private :

        QHash <QString,SystemVariable*> m_list;
        QHash <QString,SystemVariable*> m_localList;
        bool m_serverAvailable;

    public:

        explicit VariableModelManager(QObject *parent = 0);

        Q_INVOKABLE SystemVariable* localVariable(QString name, QVariant defaultValue);
        Q_INVOKABLE void setLocalVariable(QString name, QVariant value);

        Q_INVOKABLE SystemVariable* systemVariable(QString model, QString filter);
        Q_INVOKABLE SystemVariable* requestSystemVariable(QString model, QString filter);
        Q_INVOKABLE void releaseSystemVariable(QString name);

        bool serverAvailable() const {return m_serverAvailable;}

    signals:

        void serverAvailableChanged(bool arg);
        void requestVariable(QString name, QString filter);
        void releaseVariable(QString name);

    public slots:

        void updateSystemVariable(QString name, QString filter, QVariant varContent);
        void setServerAvailable(bool available);

    private slots:


        void updateModel();
};

#endif // JSONMODEL_H
