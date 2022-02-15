#include "threadworker.h"
#include <QtCore>
#include <QDebug>
#include <QtXml>



ThreadWorker_Settings::ThreadWorker_Settings(QObject *parent, QMutex *m, int *s)
{
       counter=s;
       mutex=m;
}

void ThreadWorker_Settings::run() {

    qDebug() << "Start of saveSettings thread";
    if(checked == true){

        QSettings setting3("MyApp","MySetting");
        setting3.beginGroup("LoginWindow");
        setting3.setValue("user",true);
        setting3.endGroup();

        QSettings setting4("MyApp","MySetting");
        setting4.beginGroup("LoginWindow");
        setting4.setValue("username",username);
        setting4.endGroup();

        QSettings setting5("MyApp","MySetting");
        setting5.beginGroup("LoginWindow");
        setting5.setValue("password",password);
        setting5.endGroup();

        QSettings setting6("MyApp","MySetting");
        setting6.beginGroup("LoginWindow");
        setting6.setValue("position",position);
        setting6.endGroup();




        qDebug() << "Saved User";
    }
    else {
        QSettings setting3("MyApp","MySetting");
        setting3.beginGroup("LoginWindow");
        setting3.setValue("user",false);
        setting3.endGroup();


    }

    qDebug() << "End of saveSettings thread";

    mutex->lock();
    *counter=*counter+1;
    mutex->unlock();
}

void ThreadWorker_Settings::setThread(bool _checked, QString _username, QString _password, QRect _position) {

    checked = _checked;
    username = _username;
    password = _password;
    position = _position;
}

ThreadWorker_Xml::ThreadWorker_Xml(QObject *parent, QMutex *m, int *s)
{
    counter=s;
    mutex=m;
}

void ThreadWorker_Xml::run() {

    qDebug() << "Start of saveXml thread";

    QFile file(_file);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {

            qDebug() << "Failed to open file";


        }

        QDomDocument document;
        if (!document.setContent(&file))
        {
            qDebug() << "failed to parse file";
            file.close();
        }

        file.close();

        QDomElement docEle = document.documentElement();
        QDomNodeList elements = docEle.elementsByTagName("Logs");

        QDomElement log = document.createElement( "Log" );


        QString date_string = QDateTime::currentDateTime().date().toString();
        QString date_formatted = QDate::fromString(date_string,"ddd MMM d yyyy").toString("dd.MM.yyyy");
        log.setAttribute("Username",username);
        log.setAttribute("Date",date_formatted);
        QString time = QDateTime::currentDateTime().time().toString();
        log.setAttribute("Time",time);


        docEle.appendChild(log);
        QFile outFile(_file);
        if( !outFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
        {
            qDebug( "Failed to open file for writing." );
        }

        QTextStream stream( &outFile );
        stream << document.toString();
        outFile.close();
        qDebug() << "End of saveXml thread";
        mutex->lock();
        *counter=*counter+1;
        mutex->unlock();

}

void ThreadWorker_Xml::setThread(QString _file_, QString _username) {

    _file = _file_;
    username = _username;
}

ThreadWorker_Json::ThreadWorker_Json(QObject *parent, QMutex *m, int *s)
{

    counter=s;
    mutex=m;
}

void ThreadWorker_Json::run() {


    qDebug() << "Start of saveJson thread";

    ParseClass json;
    json.createJsonFile(_file,username);

    qDebug() << "End of saveJson thread";

    mutex->lock();
    *counter=*counter+1;
    mutex->unlock();
}

void ThreadWorker_Json::setThread(QString _file_, QString _username) {
    QMutex mutex;
    mutex.lock();
    _file = _file_;
    username = _username;
    mutex.unlock();
}


