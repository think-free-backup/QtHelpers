#include "log.h"

QString Log::s_htmlPath = "";
bool Log::s_useHtml = false;

Log::Log(QObject *parent) : QObject(parent)
{

}

void Log::setHtmlPath(QString path, QString name){

    s_htmlPath = path;

    // TODO : Init path here

    QFile outFile(s_htmlPath);
    outFile.open(QIODevice::WriteOnly);

    QTextStream textStream(&outFile);

    textStream <<  "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?> \n";
    textStream <<  "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\"> \n";
    textStream <<  "<html><head> \n";
    textStream <<  "<title>" << name << " log file</title> \n";
    textStream <<  "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/> \n";
    textStream <<  "<style type=\"text/css\"> \n";
    textStream <<  "body { background: #1a242a; color: #b4c8d2; margin-right: 20px; margin-left: 20px; font-size: 14px; font-family: Arial, sans-serif, sans; } \n";
    textStream <<  "a { text-decoration: none; } \n";
    textStream <<  "a:link { color: #b4c8d2; } \n";
    textStream <<  "a:active { color: #ff9900; } \n";
    textStream <<  "a:visited { color: #b4c8d2; } \n";
    textStream <<  "a:hover { color: #ff9900; } \n";
    textStream <<  "h1 { text-align: center; } \n";
    textStream <<  "h2 { color: #ffffff; } \n";
    textStream <<  ".m, .w, .e, .f { padding: 3px; overflow: auto; } \n";
    textStream <<  ".m { background-color: #080c10; color: #b4c8d2; } \n";
    textStream <<  ".w { background-color: #839ca7; color: #1a242a; } \n";
    textStream <<  ".e { background-color: #ff9933; color: #1a242a; } \n";
    textStream <<  ".f { background-color: #ff0000; color: #1a242a; } \n";
    textStream <<  "</style></head><body> \n";
    textStream <<  "<h1>" << name << "  log file</h1> \n";
    textStream <<  " \n";

    s_useHtml = true;
}

void Log::write(QString function, QString log)
{
    QDateTime date = QDateTime::currentDateTime();

    Log::writeQDebug(date, log, function);

    if (s_useHtml){

        Log::writeHtml(date, log, function);
    }
}

void Log::write_color(QString function, QString log, QString color){

    QDateTime date = QDateTime::currentDateTime();

    #ifdef Q_OS_WIN
        Q_UNUSED(color)
        Log::writeQDebug(date, log, function);
    #else
        Log::writeQDebug(date, "\e[38;5;" + color + "m" + log + "\e[0m", function);
    #endif

    if (s_useHtml){

        Log::writeHtml(date, log, function);
    }
}

void Log::writeQDebug(QDateTime &date, QString log, QString function){

    #ifdef Q_OS_WIN
        qDebug("[%s] : %s (%s)", date.toString("yyyy-MM-dd hh:mm:ss zzz").toStdString().c_str(), log.toStdString().c_str(), function.toStdString().c_str());
    #else
        qDebug("\e[38;5;243m[%s] :\e[0m %s (%s)", date.toString("yyyy-MM-dd hh:mm:ss").toStdString().c_str(), log.toStdString().c_str(), function.toStdString().c_str());
    #endif
}

void Log::writeHtml(QDateTime &date, QString log, QString function){

    QFile outFile(s_htmlPath);
    outFile.open(QIODevice::Append);

    QTextStream textStream(&outFile);

    textStream << "<div class=\"m\"><span style=\"color:#b4c8d2;\">" << date.toString("hh:mm:ss") << "</span> : " << log << " <span style=\"color:#333333;\">[" << function << "]</span></div> \n";
}
