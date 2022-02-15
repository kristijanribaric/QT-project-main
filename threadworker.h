#ifndef THREADWORKER_H
#define THREADWORKER_H

#include <QThread>
#include <QObject>
#include <QString>
#include "parseclass.h"
#include <QRect>
#include <QMutex>



class ThreadWorker_Settings : public QThread
{
     Q_OBJECT
public:
    explicit ThreadWorker_Settings(QObject *parent,QMutex* m,int* s);
    bool checked;
    QString username;
    QString password;
    QRect position;
    void run();
    void setThread(bool _checked, QString _username, QString _password, QRect _position);


private:
    QMutex* mutex;
    int* counter;

};


class ThreadWorker_Xml : public QThread
{
     Q_OBJECT
public:
    explicit ThreadWorker_Xml(QObject *parent,QMutex* m,int* s);
    QString _file;
    QString username;
    void run();
    void setThread(QString _file_, QString _username);


private:
    QMutex* mutex;
    int* counter;


};

class ThreadWorker_Json : public QThread
{
     Q_OBJECT
public:
    explicit ThreadWorker_Json(QObject *parent,QMutex* m,int* s);
    QString _file;
    QString username;
    void run();
    void setThread(QString _file_, QString _username);

private:
    QMutex* mutex;
    int* counter;

};

#endif // THREADWORKER_H
