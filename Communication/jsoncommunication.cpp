#include "jsoncommunication.h"

JsonCommunication::JsonCommunication(QStringList host, int port, QObject *parent) : QObject(parent)
{
    m_hostMaxIdx = host.count()-1 ;
    m_currentHostIdx = 0;
    m_host = host;
    m_port = port;
    m_connected = false;

    m_regex.setPattern(":::0:::(.*):::1:::");
    m_regex.setMinimal(true);

    this->createSocket();

    m_connectionCheckerTimer = new QTimer(this);
    connect(m_connectionCheckerTimer, SIGNAL(timeout()), this, SLOT(connectionChecker()));
    m_connectionCheckerTimer->start(1000);
}


// Properties

bool JsonCommunication::connected() const
{
    return m_connected;
}

void JsonCommunication::setConnected(bool arg)
{
    if (m_connected != arg){

        m_connected = arg;

        emit connectedChanged(arg);

        if (arg)
            emit serverConnected();
        else
            emit serverDisconnected();
    }
}


// Socket creation

void JsonCommunication::createSocket(){

    m_dataStr = "";
    m_socket = new QTcpSocket(this);
    m_socket->setSocketOption(QAbstractSocket::LowDelayOption , 1);
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(messageReceived()));
}

// Connection checked : Ensure we are connected to server (Check that the socker is opened and try to open it if not)

void JsonCommunication::connectionChecker()
{
    if (!this->connected() || m_socket->state() != QAbstractSocket::ConnectedState)
    {
        this->setConnected(false);

        m_socket->deleteLater();
        this->createSocket();

        m_socket->connectToHost(m_host.at(m_currentHostIdx),m_port);
        m_socket->waitForConnected(2500);

        if (m_socket->state() != QAbstractSocket::ConnectedState)
        {
            logm("Can't connect to server : " + m_host.at(m_currentHostIdx));

            this->setConnected(false);

            // Try next host

            if (m_currentHostIdx < m_hostMaxIdx)
                m_currentHostIdx++;
            else
                m_currentHostIdx = 0;
        }
        else
        {
            logm("Client is now connected to server : " + m_host.at(m_currentHostIdx));

            this->setConnected(true);

            // Reinit host list

            m_currentHostIdx = 0;
        }
    }
}

void JsonCommunication::forceDisconnect()
{
    this->setConnected(false);

    m_socket->disconnectFromHost();
}


// Send / receive messages

void JsonCommunication::serverRequest(QString request)
{
    if (!request.contains("\"hb\""))
        dbgc("Sending to socket : " + request, "2");

    QByteArray packet;
    packet.append(":::0:::");
    packet.append(request);
    packet.append(":::1:::");

    if (this->connected() && m_socket->state() == QTcpSocket::ConnectedState){

        m_socket->write(packet);
    }
    else{

        logm("Can't write to server, forcing disconnection (" + request + ")");
        forceDisconnect();
    }

    // log("Requesting to server : " + request);
}

void JsonCommunication::messageReceived()
{
    QByteArray datagram = m_socket->readAll();

    m_dataStr.append(datagram);

    while (m_regex.indexIn(m_dataStr) > -1){

        if (!m_regex.cap(1).contains("\"hbAck\""))
            logc("Received from server : " + m_regex.cap(1) ,"1");

        emit jsonReceived(m_regex.cap(1));
        m_dataStr.remove(m_regex.cap(0));
    }

//    QStringList dataList = m_dataStr.split(":::0:::", QString::SkipEmptyParts);

//    foreach (QString data, dataList) {


//        if (data.contains(":::1:::")){

//            QString mes = data.remove(":::1:::");

//            if (!mes.contains("\"hbAck\""))
//                logc("Received from server : " + mes ,"1");

//            emit jsonReceived(mes);
//        }
//        else{

//            m_dataStr = data;
//            return;
//        }
//    }

//    m_dataStr = "";
}
