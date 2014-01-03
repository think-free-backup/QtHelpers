#include "systemvariable.h"

SystemVariable::SystemVariable(QObject *parent) : QObject(parent)
{

}

SystemVariable::SystemVariable(QString name, QVariant json, QString option, QObject *parent) : QObject(parent)
{
    setname(name);
    setVariable(json);
    setOption(option);
}
