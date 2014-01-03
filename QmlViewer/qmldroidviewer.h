#ifndef QMLDROIDVIEWER_H
#define QMLDROIDVIEWER_H

#include <QObject>
#include <QKeyEvent>
#include <QDebug>

#include "../qtquick2applicationviewer/qtquick2applicationviewer.h"

class QmlDroidViewer : public QtQuick2ApplicationViewer
{
    Q_OBJECT

    public:

        explicit QmlDroidViewer(QWindow *parent = 0);
    
    signals:
        void backKeyPressed();
        void menuKeyPressed();

    public slots:

    private :

        void keyReleaseEvent(QKeyEvent *event);
    
};

#endif // QMLDROIDVIEWER_H
