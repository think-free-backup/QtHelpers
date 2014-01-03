#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QDebug>
#include <QMetaObject>
#include <QDateTime>

class Log : public QObject
{
    Q_OBJECT

    public:

        explicit Log(QObject *parent = 0);

    signals:

    public slots:

        static void write(QString function,  QString log);
    
};

#define log(text) Log::write(Q_FUNC_INFO, text);

#endif // LOG_H
