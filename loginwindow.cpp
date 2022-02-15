#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "threadworker.h"
#include "parseclass.h"
#include "cipher.h"
#include <QCheckBox>
#include <QtSql>
#include <QSqlDatabase>
#include <QStringList>
#include <QDateTime>
#include <QtXml>
#include <QThread>



LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Prijava"));
    ui->lpassword->setVisible(false);
    LoadSettings();





}

LoginWindow::~LoginWindow()
{
    threadSettings->quit();
    threadXml->quit();
    threadJson->quit();
    delete ui;
}


void LoginWindow::on_login_clicked()
{

    QString username = ui->username->text();
    QString password = ui->password->text();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("db/ntp_database.db");



    if(db.open()){
        QSqlQuery query;
        query.prepare("SELECT username,password FROM users WHERE username = :username and password = :password ");
        query.bindValue(":username",username);
        query.bindValue(":password",password);

        if(query.exec()){

                if(query.next()){
                        QString usernameDB = query.value(0).toString();
                        QString passwordDB = query.value(1).toString();

                        if(usernameDB == username && passwordDB == password){

                            QMutex mutex;
                            threadSettings = new ThreadWorker_Settings(this,&mutex,&succes);
                            threadXml = new ThreadWorker_Xml(this,&mutex,&succes);
                            threadJson = new ThreadWorker_Json(this,&mutex,&succes);



                            threadSettings->setThread(ui->rememberMe->isChecked(),username,password, this->geometry());
                            threadXml->setThread("login_log.xml",usernameDB);
                            threadJson->setThread("login_log2.json",usernameDB);
                            threadSettings->start();
                            threadXml->start();
                            threadJson->start();

                            db.close();
                            db.removeDatabase("db/ntp_database.db");

                            this->close();
                            main = new MainWindow();
                            main->getUsername(usernameDB);
                            main->successfulThreads(succes);
                            main->show();
                        }

                }
                else {
                    QMessageBox::warning(this,tr("Prijava"),tr("Korisničko ime ili lozinka su neispravni!"));
                    db.close();
                    db.removeDatabase("db/ntp_database.db");
                }


        }
        else {

            QMessageBox::warning(this,tr("Prijava"),tr("Izvšavanje zahtjeva je neuspješno!"));
        }

    }
    else {
        QMessageBox::warning(this,tr("Prijava"),tr("Baza podataka nije otvorena"));
    }


}

void LoginWindow::LoadSettings()
{

    QSettings setting3("MyApp","MySetting");
    setting3.beginGroup("LoginWindow");
    bool login = setting3.value("user").toBool();
    if(login == true){
        QSettings setting4("MyApp","MySetting");
        setting4.beginGroup("LoginWindow");
        QString username_load = setting4.value("username").toString();

        QSettings setting5("MyApp","MySetting");
        setting5.beginGroup("LoginWindow");
        QString password_load = setting5.value("password").toString();

        QSettings setting6("MyApp","MySetting");
        setting6.beginGroup("LoginWindow");
        QRect rect = setting6.value("position").toRect();
        setGeometry(rect);
        ui->username->insert(username_load);
        ui->password->insert(password_load);
        ui->rememberMe->setChecked(true);


    } else if(login == false){
        ui->username->insert("");
        ui->password->insert("");
    }
    setting3.endGroup();

    qDebug() << "Loaded User";


}



void LoginWindow::on_last_login_clicked()
{

    QDomDocument document;
    QFile file("login_log.xml");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open XML file for reading";

    }
    else {
        if(!document.setContent(&file)) {
            qDebug() << "Failed to load XML document";
        }
        file.close();
    }
    QDomElement root = document.firstChildElement();
    QDomNodeList items = root.elementsByTagName("Log");

    QDomNode itemnode = items.at(items.count() - 1);
    if(itemnode.isElement()){
        QDomElement item_element = itemnode.toElement();

        QString username_string = item_element.attribute("Username");
        QString date_string = item_element.attribute("Date");
        QString time_string = item_element.attribute("Time");

        QString last_login_info = QString(tr("Zadnji prijavljeni korisnik je \"%1\" datuma %2 u %3")).arg(username_string).arg(date_string).arg(time_string);
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Zadnja prijava (XML)"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(last_login_info);
        msgBox.exec();


    }


}


void LoginWindow::on_last_login2_clicked()
{
    ParseClass json;
    json.parseJsonFile("login_log2.json");
}

