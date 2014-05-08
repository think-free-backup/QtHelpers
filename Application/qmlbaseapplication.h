#ifndef QMLBASEAPPLICATION_H
#define QMLBASEAPPLICATION_H

#include <QObject>
#include <QWidget>
#include <QQmlContext>

#include "../QmlViewer/qmldroidviewer.h"
#include "../Communication/networkmanager.h"
#include "../VariableModel/variablemodelmanager.h"
#include "../VariableModel/systemvariable.h"
#include "../PlatformInfo/platforminfo.h"
#include "../ShortCut/shortcut.h"

class QmlBaseApplication : public QWidget
{
    Q_OBJECT

    public:
        explicit QmlBaseApplication(QString qmlfile, QString app, QWidget *parent = 0);

    signals:

    public slots:
        void show(int w = 1000, int h = 600);
        QQmlContext *getContext();
        void setContextProperty(const QString &name, QObject * value);
        void setPackage(QString package);
        void setConnected(bool connected);

    private:

        NetworkManager * m_networkManager;
        VariableModelManager * m_model;
        PlatformInfo * m_osInfo;
        QmlDroidViewer * m_viewer;
        QString m_qmlFile;
        QQmlContext *m_context;
};

#endif // QMLBASEAPPLICATION_H
