#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QDebug>
#include <QMetaObject>
#include <QDateTime>
#include <QString>
#include <QFile>
#include <QDir>

#define HTMLPATH "./logs/"

class Log : public QObject
{
    Q_OBJECT

    public:

        explicit Log(QObject *parent = 0);

        static void write(QString function,  QString log, QString file = "portal-log.html");
        static void write_color(QString function,  QString log , QString color, QString file = "portal-log.html");
        static void initHtmlLog(QString name = "", QString file = "portal-log.html");

    private:

        static void writeHtml(QDateTime &date, QString log, QString function, QString file);
        static void writeQDebug(QDateTime &date, QString log, QString function);
};

// Log macros

#define logm(text) Log::write(Q_FUNC_INFO, text)
#define logc(text, color) Log::write_color(Q_FUNC_INFO, text, color)
#define logfile(text, file) Log::write(Q_FUNC_INFO, text, file)

// Debug macros

#ifdef LOGDEBUG

    #define dbg(text) Log::write(Q_FUNC_INFO, text)
    #define dbgc(text, color) Log::write_color(Q_FUNC_INFO, text, color)

#else

    #define dbg(text)
    #define dbgc(text, color)

#endif

#endif // LOG_H
