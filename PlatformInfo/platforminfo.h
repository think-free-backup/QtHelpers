#ifndef PLATFORMMANAGER_H
#define PLATFORMMANAGER_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QSettings>

#if defined(QT_ANDROIDEXTRAS_LIB)
    #include <QAndroidJniObject>
#endif

#include <stdlib.h>

#include "../Log/log.h"

class PlatformInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString platform READ platform NOTIFY platformChanged)
    Q_PROPERTY(QString storagePath READ storagePath NOTIFY storagePathChanged)
    Q_PROPERTY(QString configFilePath READ configFilePath NOTIFY configFilePathChanged)
    Q_PROPERTY(bool orientationPortrait READ orientationPortrait WRITE setOrientationPortrait NOTIFY orientationPortraitChanged)
    Q_PROPERTY(bool tactileScreen READ tactileScreen NOTIFY tactileScreenChanged)

    QString m_platform;
    QString m_storagePath;
    QString m_configFilePath;
    bool m_orientationPortrait;
    bool m_tactileScreen;
    QString m_package;

    public:

        explicit PlatformInfo(QString app, QObject *parent = 0);

        QString platform() const {return m_platform;}

        QString storagePath() const {return m_storagePath;}

        QString configFilePath() const {return m_configFilePath;}

        bool orientationPortrait() const {return m_orientationPortrait;}

        bool tactileScreen() const {return m_tactileScreen;}

        Q_INVOKABLE QVariant getSetting(QString key, QVariant deflt);
        Q_INVOKABLE void setSetting(QString key, QVariant value);

    public slots:

        void setOrientationPortrait(bool arg)
        {
            if (m_orientationPortrait != arg) {
                m_orientationPortrait = arg;
                emit orientationPortraitChanged(arg);
            }
        }

        void setStoragePath(QString arg)
        {
            if (m_storagePath != arg && arg != "") {

                QDir dir;
                dir.mkdir(arg);
                m_storagePath = arg;
                emit storagePathChanged(arg);
            }
        }

        void setConfigFilePath(QString arg)
        {
            if (m_configFilePath != arg && arg != "") {
                m_configFilePath = arg;
                emit configFilePathChanged(arg);
            }
        }

        void notify(QString message);
        void setPackage(QString package);
        static void notify(QString message, QString package, int priority = 0);

    signals:

        void platformChanged(QString arg);

        void storagePathChanged(QString arg);

        void configFilePathChanged(QString arg);

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

        void setTactileScreen(bool arg)
        {
            if (m_tactileScreen != arg) {
                m_tactileScreen = arg;
                emit tactileScreenChanged(arg);
            }
        }
};

#endif // PLATFORMMANAGER_H
