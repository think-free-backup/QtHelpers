#include "qmldroidviewer.h"

QmlDroidViewer::QmlDroidViewer(QWindow *parent) : QtQuick2ApplicationViewer(parent)
{
}

void QmlDroidViewer::keyReleaseEvent(QKeyEvent *event)
{
    static bool accepted=true;

    if (event->key()==Qt::Key_Close)
    {
        emit backKeyPressed();
        event->setAccepted(accepted);
    }
    else if (event->key() == Qt::Key_Menu){
        emit menuKeyPressed();
        event->setAccepted(accepted);
    }
}
