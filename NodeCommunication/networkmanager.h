#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "nodejscommunication.h"
#include "heartbeatmanager.h"

#include "../Log/log.h"

class NetworkManager : public NodeJsCommunication
{
    Q_OBJECT

    Q_PROPERTY(bool logged READ logged WRITE setLoggedState NOTIFY loggedChanged)
    Q_PROPERTY(QString ssid READ ssid WRITE setssid NOTIFY ssidChanged)

    public:

        explicit NetworkManager(QString host,QObject *parent = 0);

        // Propertie

        bool logged() const
        {
            return m_logged;
        }
        QString ssid() const
        {
            return m_ssid;
        }

    signals:

        void systemVariableChanged(QString name, QString filter, QVariant varContent);
        void callRequest(QString module, QString fct, QString param);
        void hearthbeatReceived(QString hearthbeat);
        void jsonReceived(QString type, QJsonValue body);

        // Propertie

        void loggedChanged(bool arg);
        void ssidChanged(QString arg);

    public slots:

        void requestVariable(QString name, QString filter);
        void releaseVariable(QString name);
        Q_INVOKABLE void writeVariable(QString name, QVariant value);
        Q_INVOKABLE void call(QString module, QString function, QString params);

        // Propertie

        void setLoggedState(bool arg)
        {
            m_logged = arg;
            emit loggedChanged(arg);
        }
        void setLogged() {setLoggedState(true);}
        void setNotLogged() {setLoggedState(false);}
        void setssid(QString arg)
        {
            if (m_ssid != arg) {
                m_ssid = arg;
                emit ssidChanged(arg);
            }
        }

    private slots:

        void tryLogin();
        void processJson(QString json);

    private:

        HeartbeatManager *m_heartbeatManager;

        // Propertie

        bool m_logged;
        QString m_ssid;
};

#endif // NETWORKMANAGER_H
