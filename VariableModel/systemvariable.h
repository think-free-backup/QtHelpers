#ifndef JSONSTRING_H
#define JSONSTRING_H

#include <QObject>
#include <QVariant>

class SystemVariable : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setname NOTIFY nameChanged)
    Q_PROPERTY(QVariant variable READ variable WRITE setVariable NOTIFY variableChanged)
    Q_PROPERTY(QString option READ option WRITE setOption NOTIFY optionChanged)

    private:

        QVariant m_variable;
        QString m_option;

        QString m_name;

    public:
        explicit SystemVariable(QObject *parent = 0);
        explicit SystemVariable(QString name, QVariant variable, QString option, QObject *parent = 0);

        QVariant variable() const
        {
            return m_variable;
        }

        QString option() const
        {
            return m_option;
        }

        QString name() const
        {
            return m_name;
        }

    signals:

        void variableChanged(QVariant arg);

        void optionChanged(QString arg);

        void nameChanged(QString arg);

    public slots:
    
        void setVariable(QVariant arg)
        {
            if (m_variable != arg) {
                m_variable = arg;
                emit variableChanged(arg);
            }
        }

        void setOption(QString arg)
        {
            if (m_option != arg) {
                m_option = arg;
                emit optionChanged(arg);
            }
        }
        void setname(QString arg)
        {
            if (m_name != arg) {
                m_name = arg;
                emit nameChanged(arg);
            }
        }
};

#endif // JSONSTRING_H
