#include "log.h"

Log::Log(QObject *parent) : QObject(parent)
{

}

void Log::write(QString function, QString log)
{
    QDateTime date = QDateTime::currentDateTime();

    qDebug("[%s] : %s (%s)", date.toString("yyyy-MM-dd hh:mm:ss").toStdString().c_str(), log.toStdString().c_str(), function.toStdString().c_str());
}
