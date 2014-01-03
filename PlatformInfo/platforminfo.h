#ifndef PLATFORMMANAGER_H
#define PLATFORMMANAGER_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QSettings>

#include "../Log/log.h"

#include <stdlib.h>

class PlatformInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString platform READ platform NOTIFY platformChanged)
    Q_PROPERTY(QString storagePath READ storagePath NOTIFY storagePathChanged)
    Q_PROPERTY(bool orientationPortrait READ orientationPortrait WRITE setOrientationPortrait NOTIFY orientationPortraitChanged)
    Q_PROPERTY(bool tactileScreen READ tactileScreen NOTIFY tactileScreenChanged)

    QString m_platform;

    QString m_storagePath;

    bool m_orientationPortrait;

    bool m_tactileScreen;

    public:

        explicit PlatformInfo(QString app, QObject *parent = 0);

        QString platform() const {return m_platform;}

        QString storagePath() const {return m_storagePath;}

        bool orientationPortrait() const {return m_orientationPortrait;}

        bool tactileScreen() const {return m_tactileScreen;}

        QString getSetting(QString key, QString deflt);

    public slots:

        void setOrientationPortrait(bool arg)
        {
            if (m_orientationPortrait != arg) {
                m_orientationPortrait = arg;
                emit orientationPortraitChanged(arg);
            }
        }

    signals:

        void platformChanged(QString arg);

        void storagePathChanged(QString arg);

        void orientationPortraitChanged(bool arg);

        void tactileScreenChanged(bool arg);

    private slots:

        void setPlatform(QString arg)
        {
            if (m_platform != arg) {
                m_platform = arg;
                emit platformChanged(arg);
            }
        }

        void setStoragePath(QString arg)
        {
            if (m_storagePath != arg) {
                m_storagePath = arg;
                emit storagePathChanged(arg);
            }
        }

        void setTactileScreen(bool arg)
        {
            if (m_tactileScreen != arg) {
                m_tactileScreen = arg;
                emit tactileScreenChanged(arg);
            }
        }
};

#endif // PLATFORMMANAGER_H
