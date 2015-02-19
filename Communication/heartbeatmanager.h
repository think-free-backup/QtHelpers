#ifndef HEARTBEATMANAGER_H
#define HEARTBEATMANAGER_H

#include <QObject>
#include <QTimer>
#include <QUuid>
#include <QQueue>
#include <QThread>

#include "../Log/log.h"

class HeartbeatManager : public QThread
{
    Q_OBJECT

    public:
        explicit HeartbeatManager(int queueSize = 3, int interval = 3000, QObject *parent = 0);

    signals:
        void queueFull();
        void networkRequest(QString request);

    public slots:
        void stop();
        void setRunning(bool running);
        void validate(QString uuid);

    private:
        QTimer *m_timer;
        int m_queueSize;
        QQueue<QString> m_hbQueue;
        int m_inteval;
    
    private slots:
        void sendHeartbeat();
        void enqueue(QString uuid);

    protected:
        void run();
};

#endif // HEARTBEATMANAGER_H
