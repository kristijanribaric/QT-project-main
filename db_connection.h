#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QObject>
#include <QString>
#include <QMessageBox>
#include <QtDebug>


#endif // DB_CONNECTION_H

class Connection_login {
private:
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QSqlQuery query;
    QString username;
    QString password;
    QString username_res;
    QString password_res;
    QDebug debug;

public:

    void setBase(QString _path) {
        db.setDatabaseName(_path);
    }

    bool isOpen() {
        if (db.open()) {
            return true;
        }else return false;

    }


    bool fetchUser(QString _username, QString _password) {
        username = _username;
        password = _password;
        query.prepare("select username,password where username = :username limit 1");
        query.bindValue(":username",username);
        query.exec();
        if(query.next()){
            if(query.value(1).toString() == password ){
                return 1;
            }
        }
    }

};


