#ifndef THMANAGER_H
#define THMANAGER_H

#include <QObject>
#include <QThread>

#include <QDebug>

class ThManager : public QObject
{
    Q_OBJECT

    public:
        explicit ThManager(QObject *parent = 0);
        ~ThManager();

    signals:

    private:

        QList<QThread*> m_threads;
        QList<QObject*> m_threadsObject;

    public slots:

        void createThreadForObject(QObject *obj);
        void stop();
};

#endif // THMANAGER_H
