#ifndef PARSECLASS_H
#define PARSECLASS_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QIODevice>
#include <QStringList>
#include <QDebug>
#include <QList>
#include <QMessageBox>

class ParseClass
{
private:
    QString file_name;
    QString username;
    QString date;
    QString time;
public:

    void createJsonFile(const QString _fileName,QString _username);;
    void parseJsonFile(const QString _fileName);;

};

#endif // PARSECLASS_H
