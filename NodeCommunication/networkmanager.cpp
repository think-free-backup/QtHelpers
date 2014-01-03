#include "networkmanager.h"

NetworkManager::NetworkManager(QString host, QObject *parent) : NodeJsCommunication(host,parent)
{
    m_logged = false;

    m_heartbeatManager = new HeartbeatManager(3,this);
        connect(m_heartbeatManager, SIGNAL(networkRequest(QString)), this, SLOT(serverRequest(QString)));
        connect(m_heartbeatManager, SIGNAL(queueFull()), this, SLOT(forceDisconnect()));
        connect(this,SIGNAL(connectedChanged(bool)),m_heartbeatManager,SLOT(setRunning(bool)));
        connect(this,SIGNAL(hearthbeatReceived(QString)),m_heartbeatManager,SLOT(validate(QString)));

    connect(this,SIGNAL(jsonReceived(QString)),this,SLOT(processJson(QString)));
    connect(this,SIGNAL(serverConnected()),this,SLOT(tryLogin()));
    connect(this,SIGNAL(serverDisconnected()),this,SLOT(setNotLogged()));
}

void NetworkManager::requestVariable(QString name, QString filter)
{
    QString str = QString("{\"type\" : \"call\", \"body\" : { \"module\" : \"variableManager\", \"function\" : \"request\", \"param\" : [\"%1\",\"%2\"] } }")
            .arg(name)
            .arg(filter);

    serverRequest(str);
}

void NetworkManager::releaseVariable(QString name)
{
    QString str = QString("{\"type\" : \"call\", \"body\" : { \"module\" : \"variableManager\", \"function\" : \"release\", \"param\" : [\"%1\"] } }")
            .arg(name);

    serverRequest(str);
}

void NetworkManager::writeVariable(QString name, QVariant value)
{
    QString str = QString("{\"type\" : \"call\", \"body\" : { \"module\" : \"variableManager\", \"function\" : \"write\", \"param\" : \"[\"%1\",\"%2\"]\" } }")
            .arg(name)
            .arg(value.toString());

    serverRequest(str);
}

void NetworkManager::call(QString module, QString function, QString params)
{
    QString str = QString("{\"type\" : \"call\", \"body\" : { \"module\" : \"%1\", \"function\" : \"%2\", \"param\" : [%3] } }")
            .arg(module)
            .arg(function)
            .arg(params);

    serverRequest(str);
}

void NetworkManager::tryLogin()
{
    if (m_ssid != "")
        serverRequest("{ \"type\" : \"session\", \"body\" : {\"function\" : \"tryValidateSession\", \"param\" : [\"" + m_ssid + "\"] } }");
}

void NetworkManager::processJson(QString json)
{

    // Parse main parts of the message (type and body)

    QJsonDocument doc; doc = doc.fromJson(json.toUtf8());
    QString type = doc.object().take("type").toString();
    QJsonValue body = doc.object().take("body");

    // Routing messages by type

    if ( type == "HB-ACK" ){

        emit hearthbeatReceived(body.toString());
    }
    else if ( type == "setVariable" ){

        QString variable = body.toObject().take("variable").toString();
        QString option = body.toObject().take("option").toString();
        QString json = QJsonDocument( body.toObject().take("value").toObject() ).toJson();

        emit systemVariableChanged(variable, option, json);
    }
    else if ( type == "call" ){

            QString module = body.toObject().take("module").toString();
            QString function = body.toObject().take("function").toString();
            QString params = QJsonDocument( body.toObject().take("param").toArray() ).toJson();

            emit callRequest(module, function, params);
    }
    else if ( type == "ssid" ){

        setssid(body.toString());

        setLoggedState(true);
    }
    else if ( type == "login-error" ){

        log("Login error : " + body.toString());
        setLoggedState(false);
    }
    else if ( type == "error"){

        log("Server error : " + body.toString());
    }
    else{

        log("Unkown message received : " + json);
        emit jsonReceived(type, body);
    }
}

