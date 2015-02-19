#include "heartbeatmanager.h"

HeartbeatManager::HeartbeatManager(int queueSize, int interval, QObject *parent) : QThread(parent)
{
    m_queueSize = queueSize;
    m_inteval = interval;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(sendHeartbeat()));
}

void HeartbeatManager::run(){

    while(1){

        msleep(m_inteval);
        sendHeartbeat();
    }
}

void HeartbeatManager::stop()
{
    quit();
}

void HeartbeatManager::setRunning(bool running)
{
    if (running)
        start();
    else
        stop();
}

void HeartbeatManager::sendHeartbeat()
{
    QString uuid = QUuid::createUuid().toString().replace("{","").replace("}","");
    enqueue(uuid);
    emit networkRequest("{\"type\" : \"hb\", \"body\" : \"" + uuid + "\"}");
}

void HeartbeatManager::enqueue(QString uuid)
{
    if (m_queueSize != 0){

        if (m_hbQueue.length() == m_queueSize){

            logm("Heartbeat queue is full");
            emit queueFull();
            m_hbQueue.clear();
        }
        else{

            m_hbQueue.enqueue(uuid);
        }
    }
}

void HeartbeatManager::validate(QString uuid)
{
    if (m_queueSize != 0){
        m_hbQueue.takeAt(m_hbQueue.indexOf(uuid));
    }
}
