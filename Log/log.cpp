#include "log.h"

Log::Log(QObject *parent) : QObject(parent)
{

}

void Log::write(QString function, QString log)
{
    QDateTime date = QDateTime::currentDateTime();

    #ifdef DONTLOGCOLOR

        qDebug("[%s] : %s (%s)", date.toString("yyyy-MM-dd hh:mm:ss").toStdString().c_str(), log.toStdString().c_str(), function.toStdString().c_str());

    #else

        qDebug("\e[38;5;246m[%s] :\e[0m %s \e[38;5;238m(%s)\e[0m", date.toString("yyyy-MM-dd hh:mm:ss").toStdString().c_str(), log.toStdString().c_str(), function.toStdString().c_str());

    #endif
}
