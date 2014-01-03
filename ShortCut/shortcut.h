#ifndef SHORTCUT_H
#define SHORTCUT_H

#include <QQuickItem>

class Shortcut : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant key READ key WRITE setKey NOTIFY keyChanged)
public:
    explicit Shortcut(QObject *parent = 0);

    void setKey(QVariant key);
    QVariant key() { return m_keySequence; }

    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void keyChanged();
    void activated();
    void pressedAndHold();

public slots:

private:
    QKeySequence m_keySequence;
    bool m_keypressAlreadySend;
};

#endif // SHORTCUT_H
