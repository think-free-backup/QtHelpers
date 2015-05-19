#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QDebug>
#include <QMetaObject>
#include <QDateTime>
#include <QString>
#include <QFile>

class Log : public QObject
{
    Q_OBJECT

    public:

        explicit Log(QObject *parent = 0);

        static void write(QString function,  QString log);
        static void write_color(QString function,  QString log , QString color);
        static void setHtmlPath(QString path, QString name = "");

    private:

        static void writeHtml(QDateTime &date, QString log, QString function);
        static void writeQDebug(QDateTime &date, QString log, QString function);

        static QString s_htmlPath;
        static bool s_useHtml;
};

// Log macros

#define logm(text) Log::write(Q_FUNC_INFO, text);
#define logc(text, color) Log::write_color(Q_FUNC_INFO, text, color);

// Debug macros

#ifdef LOGDEBUG

    #define dbg(text) Log::write(Q_FUNC_INFO, text);
    #define dbgc(text, color) Log::write_color(Q_FUNC_INFO, text, color);

#else

    #define dbg(text);
    #define dbgc(text, color);

#endif

#endif // LOG_H
