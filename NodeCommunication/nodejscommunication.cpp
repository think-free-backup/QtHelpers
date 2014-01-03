#include "nodejscommunication.h"

NodeJsCommunication::NodeJsCommunication(QString host,QObject *parent) : QObject(parent)
{
    m_host = host;
    m_connected = false;

    m_socket = new QTcpSocket(this);
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(messageReceived()));

    m_connectionCheckerTimer = new QTimer(this);
    connect(m_connectionCheckerTimer, SIGNAL(timeout()), this, SLOT(connectionChecker()));
    m_connectionCheckerTimer->start(1000);
}


// Properties

bool NodeJsCommunication::connected() const
{
    return m_connected;
}

void NodeJsCommunication::setConnected(bool arg)
{
    m_connected = arg;

    emit connectedChanged(arg);

    if (arg)
        emit serverConnected();
    else
        emit serverDisconnected();
}


// Connection checked : Ensure we are connected to server (Check that the socker is opened and try to open it if not)

void NodeJsCommunication::connectionChecker()
{
    if (m_socket->state() != QAbstractSocket::ConnectedState)
    {
        //log("Client is not connected");

        this->setConnected(false);

        m_socket->connectToHost(m_host,20001);
        m_socket->waitForConnected(500);

        if (m_socket->state() != QAbstractSocket::ConnectedState)
        {
            //log("Can't connect to server");

            this->setConnected(false);
        }
        else
        {
            log("Client is now connected");

            this->setConnected(true);
        }
    }
}

void NodeJsCommunication::forceDisconnect()
{
    log("Forcing disconnection");
    m_socket->disconnectFromHost();
}


// Send / receive messages

void NodeJsCommunication::serverRequest(QString request)
{
    QByteArray packet;
    packet.append(":::0:::");
    packet.append(request);

    m_socket->write(packet);
    m_socket->waitForBytesWritten();

    // log("Requesting to server : " + request);
}

void NodeJsCommunication::messageReceived()
{
    QByteArray datagram = m_socket->readAll();

    m_dataStr.append(datagram);

    QStringList dataList = m_dataStr.split(":::0:::");

    foreach (QString data, dataList) {

        if (data != ""){

            if (data.contains(":::1:::")){

                emit jsonReceived(data.remove(":::1:::"));
            }
            else{

                m_dataStr = data;
                return;
            }
        }
    }

    m_dataStr = "";
}
