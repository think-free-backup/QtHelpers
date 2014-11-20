#include "log.h"

Log::Log(QObject *parent) : QObject(parent)
{

}

void Log::write(QString function, QString log)
{
    QDateTime date = QDateTime::currentDateTime();

    #ifdef Q_OS_WIN
        qDebug("[%s] : %s (%s)", date.toString("yyyy-MM-dd hh:mm:ss zzz").toStdString().c_str(), log.toStdString().c_str(), function.toStdString().c_str());
    #else
        qDebug("\e[38;5;243m[%s] :\e[0m %s (%s)", date.toString("yyyy-MM-dd hh:mm:ss").toStdString().c_str(), log.toStdString().c_str(), function.toStdString().c_str());
    #endif
}

void Log::write_color(QString function, QString log, QString color){

    #ifdef Q_OS_WIN
        Q_UNUSED(color)
        Log::write(function, log);
    #else
        Log::write(function, "\e[38;5;" + color + "m" + log + "\e[0m");
    #endif
}
