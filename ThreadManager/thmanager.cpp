#include "thmanager.h"

ThManager::ThManager(QObject *parent) : QObject(parent){

}

ThManager::~ThManager(){

    dbg("Deleting Thread manager " + this->objectName()) ;
    stop();
}

void ThManager::createThreadForObject(QObject *obj, QString name){

    QThread *th = new QThread(this);
        th->setObjectName(name);
        obj->setParent(0);
        obj->setObjectName(name);
        connect(obj, SIGNAL(destroyed()), th, SLOT(quit()));
        connect(th, SIGNAL(finished()), th, SLOT(deleteLater()));

    obj->moveToThread(th);;
    th->start();

    m_threads.append(th);
    m_threadsObject.append(obj);
}

void ThManager::stop(){

    foreach (QObject *tho, m_threadsObject) {

        dbg("    -> detroying thread object " + tho->objectName());
        tho->deleteLater();
    }
}
