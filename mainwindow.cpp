#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "godisnji.h"
#include <QFontDialog>
#include <QFont>
#include <QtSql>
#include <QSqlDatabase>
#include <QDateTime>
#include <QPrinter>
#include <QPrintDialog>




MainWindow::MainWindow()
    : ui(new Ui::MainWindow)

{

    ui->setupUi(this);
    this->setWindowTitle(tr("Glavni prozor"));
    LoadSettings();


}




MainWindow::~MainWindow()
{
    SaveSettings();
    delete ui;
}

void MainWindow::getUsername(QString _username)
{
    username = _username;
}

void MainWindow::successfulThreads(int _counter)
{
    qDebug() << "Succesfully completed threads: " << _counter;
}

void MainWindow::readFile()
{
    QFile file("C:/example/file.txt");
    if (!file.open(QIODevice::ReadOnly))
            return;

}


void MainWindow::SaveSettings()
{
    QSettings setting("%APPDATA%/NTP_projekt/MySoft.ini",QSettings::IniFormat);
    setting.setValue("position",this->geometry());


    qDebug() << "Saved Position";
}

void MainWindow::LoadSettings()
{
    QSettings setting("%APPDATA%/NTP_projekt/MySoft.ini",QSettings::IniFormat);
    QRect rect = setting.value("position").toRect();
    setGeometry(rect);

    qDebug() << "Loaded Position";

    QSettings setting2("MyApp","MySetting");
    setting2.beginGroup("MainWindow");
    QFont theFont = qvariant_cast<QFont>(setting2.value("font"));
    setFont(theFont);
    setting2.endGroup();

    qDebug() << "Loaded Font";
}




void MainWindow::on_confirm_clicked()
{
    QString name = ui->Eime->text();
    QString lastname = ui->Eprezime->text();


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("db/ntp_database.db");



    if(db.open()){
        QSqlQuery query;
        query.prepare("select * from employees where name = :name and lastname = :lastname ");
        query.bindValue(":name",name);
        query.bindValue(":lastname",lastname);

        if(!query.exec()){
            QMessageBox::warning(this,tr("Konekcija"),tr("Upit nije uspješno izvršen!"));

        }
        else {

            if(query.next()){
                QString nameDB = query.value(1).toString();
                QString lastnameDB = query.value(2).toString();
                QString start = query.value(3).toString();
                QString end = query.value(4).toString();
                int remaining = query.value(5).toInt();

                QDate start_date = QDate::fromString(start,"ddd MMM d yyyy");
                QDate end_date = QDate::fromString(end,"ddd MMM d yyyy");

                ui->Epocetak->setDate(start_date);
                ui->Ekraj->setDate(end_date);
                Godisnji izostanak(start_date,end_date);
                int br_izbivanja = izostanak.izracunDana();
                QString alert;
                if(izostanak.prekoracenje(remaining) == true){
                    alert =  tr("Korisnik je prekoračio dopušten broj slobodnih dana.");
                } else if (izostanak.prekoracenje(remaining) == false) {
                    alert = tr("Korisnik nije prekoračio dopušten broj slobodnih dana.");
                }



                ui->preostali_dani->setText(QString(tr("Preostalo dana : %1")).arg(30 - br_izbivanja));
                ui->prekoracenje->setText(alert);
                ui->unesi->setEnabled(true);
                ui->Epocetak->setEnabled(true);
                ui->Ekraj->setEnabled(true);



            } else {
                 QMessageBox::warning(this,tr("Greška"),tr("Korisnik ne postoji u bazi"));
            }
        }
    }




}



void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,this);
    if(ok) {
        setFont(font);

        QSettings setting2("MyApp","MySetting");
        setting2.beginGroup("MainWindow");
        setting2.setValue("font",font);
        setting2.endGroup();


        qDebug() << "Saved Font";

    } else return;
}



void MainWindow::on_actionOdjava_triggered()
{
    close();
}


void MainWindow::on_unesi_clicked()
{
    if(!ui->Eime->text().isEmpty() && !ui->Eprezime->text().isEmpty() ){

        QString name = ui->Eime->text();
        QString lastname = ui->Eprezime->text();
        QDate start = ui->Epocetak->date();
        QDate end = ui->Ekraj->date();
        Godisnji izostanak(start,end);
        int br_izbivanja = izostanak.izracunDana();


        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("db/ntp_database.db");



        if(db.open()){
            QSqlQuery query;
            query.prepare("UPDATE employees set vacation_start = :start, vacation_end = :end, vacation_remaining = :remaining  WHERE name = :name and lastname = :lastname ");
            query.bindValue(":start",start.toString());
            query.bindValue(":end",end.toString());
            query.bindValue(":remaining",(30 - br_izbivanja));
            query.bindValue(":name",name);
            query.bindValue(":lastname",lastname);


            if(!query.exec()){
                QMessageBox::warning(this,tr("Konekcija"),tr("Upit nije uspješno izvršen!"));

            } else {
                QMessageBox::information(this,"Unos",QString(tr("Godišnji za korisnika %1 %2 uspješno unesen.")).arg(name).arg(lastname));
            }
        }
    } else {
        ui->unesi->setEnabled(false);
        ui->Epocetak->setEnabled(false);
        ui->Ekraj->setEnabled(false);
    }
}


void MainWindow::on_pdfReport_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("db/ntp_database.db");
    QString employees;


    if(db.open()){
        QSqlQuery query;
        query.prepare("select * from employees");

        if(!query.exec()){
            QMessageBox::warning(this,"Connection","Query exectuion failed!");

        }
        else {

            while(query.next()){
                int index = query.value(0).toInt();
                QString nameDB = query.value(1).toString();
                QString lastnameDB = query.value(2).toString();
                QString start = query.value(3).toString();
                QString end = query.value(4).toString();
                int remaining = query.value(5).toInt();

                QString start_date_formatted = QDate::fromString(start,"ddd MMM d yyyy").toString("dd.MM.yyyy");
                QString end_date_formatted = QDate::fromString(end,"ddd MMM d yyyy").toString("dd.MM.yyyy");
                employees.append(QString(tr("<tr><td>%1.</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>%6</td></tr>")).arg(index).arg(nameDB).arg(lastnameDB).arg(start_date_formatted).arg(end_date_formatted).arg(remaining));







            }
        }
    }
    QPrinter printer;
    printer.setPrinterName("printer name");
    QPrintDialog dialog(&printer,this);
    if(dialog.exec() == QDialog::Rejected) return;
    QString html = QString(tr("<h1 align='center'>Izvještaj o godišnjem odmoru zaposlenika</h1><hr><table  cellspacing='15' align='center'>"
       "<tr>"
            "<th></th>"
            "<th>Ime</th>"
            "<th>Prezime</th>"
            "<th>Početak godišnjeg</th>"
            "<th>Kraj godišnjeg</th>"
            "<th>Preostalo</th>"
        "</tr>"
        "%1"
    "</table>")).arg(employees);

        QTextDocument document;
        document.setHtml(html);
        document.print(&printer);

}


void MainWindow::on_preuzmi_clicked()
{

    HttpDownload downloader( this );
    downloader.setModal( true );
    downloader.exec();
}


void MainWindow::on_generatorButton_clicked()
{
    ClientTCP clientTCP( this );
    clientTCP.setModal( true );
    clientTCP.exec();
}




void MainWindow::on_udpButton_clicked()
{
    ClientUDP clientUDP( this );
    clientUDP.setModal( true );
    clientUDP.exec();
}


void MainWindow::on_pregled_clicked()
{
    Table table( this );
    table.setModal( true );
    table.exec();
}


void MainWindow::on_actionTrenutni_korisnik_triggered()
{
     QMessageBox::information(this,"Prijavljeni korisnik",QString(tr("Trenutno je prijavljen korisnik %1.")).arg(username));
}

