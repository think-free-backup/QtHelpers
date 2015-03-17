#include "jsoncommunication.h"

JsonCommunication::JsonCommunication(QString host, int port, QObject *parent) : QObject(parent)
{
    m_host = host;
    m_port = port;
    m_connected = false;

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

    m_socket = new QTcpSocket(this);
    m_socket->setSocketOption(QAbstractSocket::LowDelayOption , 1);
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(messageReceived()));
}

// Connection checked : Ensure we are connected to server (Check that the socker is opened and try to open it if not)

void JsonCommunication::connectionChecker()
{
    if (m_socket->state() != QAbstractSocket::ConnectedState)
    {
        this->setConnected(false);

        m_socket->deleteLater();
        this->createSocket();

        m_socket->connectToHost(m_host,m_port);
        m_socket->waitForConnected(2500);

        if (m_socket->state() != QAbstractSocket::ConnectedState)
        {
            logm("Can't connect to server");

            this->setConnected(false);
        }
        else
        {
            logm("Client is now connected");

            this->setConnected(true);
        }
    }
}

void JsonCommunication::forceDisconnect()
{
    logm("Forcing disconnection");

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

    if (m_socket->state() == QTcpSocket::ConnectedState){

        m_socket->write(packet);
        //m_socket->waitForBytesWritten();
    }
    else{

        forceDisconnect();
    }

    // log("Requesting to server : " + request);
}

void JsonCommunication::messageReceived()
{
    QByteArray datagram = m_socket->readAll();

    m_dataStr.append(datagram);

    QStringList dataList = m_dataStr.split(":::0:::");

    foreach (QString data, dataList) {

        if (data != ""){

            if (data.contains(":::1:::")){

                QString mes = data.remove(":::1:::");

                if (!mes.contains("\"hbAck\""))
                    dbgc("Received from server : " + mes ,"1");

                emit jsonReceived(mes);
            }
            else{

                m_dataStr = data;
                return;
            }
        }
    }

    m_dataStr = "";
}
