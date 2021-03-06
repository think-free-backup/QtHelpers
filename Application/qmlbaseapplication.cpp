#include "qmlbaseapplication.h"

QmlBaseApplication::QmlBaseApplication(QString qmlfile, QString app, QWidget *parent) : QWidget(parent)
{
    qmlRegisterType<SystemVariable>("org.baseapp.jsonstring", 1, 0, "JsonString");
    qmlRegisterType<Shortcut>("org.baseapp.shortcut", 1, 0, "Shortcut");

    m_qmlFile = qmlfile;
    m_osInfo = new PlatformInfo(app, this);
    m_viewer = new QmlDroidViewer();
    m_context = m_viewer->rootContext();

    m_networkManager = new NetworkManager(m_osInfo->getSetting("serverIp", "127.0.0.1").toString(), m_osInfo->getSetting("serverPort", "20001").toInt(),this);

    m_model = new VariableModelManager(this);
        m_model->setAllowServerVariableCreation(m_osInfo->getSetting("allowServerVariableCreation", "true").toBool() );
        QObject::connect(m_networkManager, SIGNAL(loggedChanged(bool)), m_model, SLOT(setServerAvailable(bool)));
        QObject::connect(m_networkManager, SIGNAL(loggedChanged(bool)), this, SLOT(setConnected(bool)));
        QObject::connect(m_networkManager, SIGNAL(systemVariableChanged(QString,QString,QVariant)), m_model, SLOT(updateSystemVariable(QString,QString,QVariant)));
        QObject::connect(m_model, SIGNAL(requestVariable(QString,QString)), m_networkManager, SLOT(requestVariable(QString,QString)));
        QObject::connect(m_model, SIGNAL(releaseVariable(QString)), m_networkManager, SLOT(releaseVariable(QString)));

    setContextProperty("variablesModel",m_model);
    setContextProperty("osInfo",m_osInfo);
    setContextProperty("network", m_networkManager);
    setContextProperty("viewer", m_viewer);
}

void QmlBaseApplication::show(int w,int h){

    m_viewer->setMainQmlFile(m_qmlFile);

    if (m_osInfo->platform() != "Android"){
        m_viewer->setHeight(h);
        m_viewer->setWidth(w);
        m_viewer->show();
    }
    else{
        m_viewer->showExpanded();
    }
}

QQmlContext * QmlBaseApplication::getContext(){

    return m_viewer->rootContext();
}

void QmlBaseApplication::setContextProperty(const QString &name, QObject *value)
{
     m_context->setContextProperty(name, value);
}

void QmlBaseApplication::setPackage(QString package){

    m_osInfo->setPackage(package);

    setConnected(m_networkManager->logged());
}

void QmlBaseApplication::setConnected(bool connected){

    if (connected)
        m_osInfo->notify("Connected");
    else
        m_osInfo->notify("Not connected");
}
