#include "platforminfo.h"

PlatformInfo::PlatformInfo(QString app,QObject *parent) : QObject(parent)
{
    // Define platform variables

    QString OS;
    QString STORAGEPATH;
    bool TACTIL;

    #ifdef Q_OS_LINUX
        OS = "Linux";
        STORAGEPATH = QDir::homePath ();
        TACTIL = false;
    #endif
    #ifdef Q_OS_WIN
        OS = "Windows";
        STORAGEPATH = QDir::homePath ();
        TACTIL = false;
    #endif
    #ifdef Q_OS_MACX
        OS = "Mac";
        STORAGEPATH = QDir::homePath ();
        TACTIL = false;
    #endif
    #ifdef Q_OS_ANDROID
        OS = "Android";
        STORAGEPATH = getenv("EXTERNAL_STORAGE");
        TACTIL = true;
    #endif
    #ifdef Q_OS_IOS
        OS = "IOS";
        QString cur_dir = QDir::currentPath();
        STORAGEPATH = cur_dir.left( cur_dir.lastIndexOf( "/" ) + 1 ).append("Documents");
        TACTIL = true;
    #endif
    // Create storage folder if doesn't exists

    STORAGEPATH.append("/.");
    STORAGEPATH.append(app);
    STORAGEPATH.append("/");

    QDir dir;
    dir.mkdir(STORAGEPATH);

    // Set platform variables

    setPlatform(OS);
    setTactileScreen(TACTIL);
    setStoragePath(STORAGEPATH);
}

QVariant PlatformInfo::getSetting(QString key, QString deflt){

    QSettings * settings = 0;
    settings = new QSettings( storagePath() + "config.ini", QSettings::IniFormat );
    return settings->value( key, deflt);
}

void PlatformInfo::notify(QString message){

    //Q_UNUSED(message)

    #if defined(QT_ANDROIDEXTRAS_LIB)

    log("Updating notification");

    QAndroidJniObject javaNotification = QAndroidJniObject::fromString(message);

    QAndroidJniObject::callStaticMethod<void>(
                                 "org/qtproject/qt5/android/bindings/QtService",
                                 "notify",
                                 "(Ljava/lang/String;)V",
                                 javaNotification.object<jstring>());
    #endif
}
