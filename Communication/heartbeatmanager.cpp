#include "heartbeatmanager.h"

HeartbeatManager::HeartbeatManager(int queueSize, int interval, QObject *parent) : QObject(parent)
{
    m_queueSize = queueSize;
    m_inteval = interval;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(sendHeartbeat()));
}

void HeartbeatManager::start()
{
    emit networkRequest("{\"type\" : \"hbNegotiation\", \"body\" : \"" + QString::number(m_inteval * 2) + "\"}");
    m_hbQueue.clear();
    m_timer->start(m_inteval);
}

void HeartbeatManager::stop()
{
    m_timer->stop();
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

            log("Heartbeat queue is full");
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
