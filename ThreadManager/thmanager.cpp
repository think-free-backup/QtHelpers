#include "thmanager.h"

ThManager::ThManager(QObject *parent) : QObject(parent)
{

}

ThManager::~ThManager(){

    qDebug() << "Deleting Thread manager " << this->objectName() ;
    stop();
}

void ThManager::createThreadForObject(QObject *obj){

    obj->setParent(0);
    QThread *th = new QThread();
    connect(th, SIGNAL(finished()), th, SLOT(deleteLater()));
    obj->moveToThread(th);
    th->start();

    m_threads.append(th);
    m_threadsObject.append(obj);
}

void ThManager::stop(){

    foreach (QThread *th, m_threads) {

        th->quit();
        th->terminate();
        th->wait(5000);
    }

    foreach (QObject *tho, m_threadsObject) {

        tho->deleteLater();
    }
}
