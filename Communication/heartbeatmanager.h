#ifndef HEARTBEATMANAGER_H
#define HEARTBEATMANAGER_H

#include <QObject>
#include <QTimer>
#include <QUuid>
#include <QQueue>

#include "../Log/log.h"

class HeartbeatManager : public QObject
{
    Q_OBJECT

    public:
        explicit HeartbeatManager(int queueSize,QObject *parent = 0);

    signals:
        void queueFull();
        void networkRequest(QString request);

    public slots:
        void start();
        void stop();
        void setRunning(bool running);
        void validate(QString uuid);

    private:
        QTimer *m_timer;
        int m_queueSize;
        QQueue<QString> m_hbQueue;
    
    private slots:
        void sendHeartbeat();
        void enqueue(QString uuid);
};

#endif // HEARTBEATMANAGER_H
