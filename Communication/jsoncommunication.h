#ifndef NODEJSCOMMUNICATION_H
#define NODEJSCOMMUNICATION_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QTcpSocket>
#include <QTimer>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "../Log/log.h"

class JsonCommunication : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

    public:

        explicit JsonCommunication(QStringList host,int port, QObject *parent = 0);

        // Properties

        bool logged() const;
        bool connected() const;

    signals:

        void jsonReceived(QString json);

        // Properties

        void connectedChanged(bool arg);
        void serverConnected();
        void serverDisconnected();

    public slots:

        Q_INVOKABLE void serverRequest(QString request);
        Q_INVOKABLE void forceDisconnect();

    private:

        QTcpSocket *m_socket;
        QStringList m_host;
        int m_port;
        QTimer *m_connectionCheckerTimer;
        QString m_dataStr;
        QRegExp m_regex;

        // Properties
        bool m_connected;

        // Host list managment

        int m_hostMaxIdx;
        int m_currentHostIdx;

    private slots:

        void createSocket();
        void messageReceived();
        void connectionChecker();

        // Properties
        void setConnected(bool arg);

};

#endif // NODEJSNETWORKMANAGER_H
