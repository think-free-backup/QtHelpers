#include "heartbeatmanager.h"

HeartbeatManager::HeartbeatManager(int queueSize, QObject *parent) : QObject(parent)
{
    m_queueSize = queueSize;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(sendHeartbeat()));
}

void HeartbeatManager::start()
{
    emit networkRequest("{\"type\" : \"HB-negotiation\", \"body\" : \"5000\"}");
    m_timer->start(3000);
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
    emit networkRequest("{\"type\" : \"HB\", \"body\" : \"" + uuid + "\"}");
}

void HeartbeatManager::enqueue(QString uuid)
{
    if (m_hbQueue.length() == m_queueSize){

        log("Queue is full");
        emit queueFull();
        m_hbQueue.clear();
    }
    else{

        m_hbQueue.enqueue(uuid);
    }
}

void HeartbeatManager::validate(QString uuid)
{
    m_hbQueue.takeAt(m_hbQueue.indexOf(uuid));
}
