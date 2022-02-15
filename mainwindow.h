#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QSettings>
#include <QDebug>
#include <QPalette>
#include <httpdownload.h>
#include <clienttcp.h>
#include <QtNetwork>
#include <QDataStream>
#include <QTcpSocket>
#include <QUdpSocket>
#include <clientudp.h>
#include <table.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();
    void getUsername( QString _username);
    void successfulThreads( int _counter);


private slots:

    void on_confirm_clicked();
    void on_actionFont_triggered();
    void on_actionOdjava_triggered();
    void on_unesi_clicked();
    void on_pdfReport_clicked();
    void readFile();
    void on_preuzmi_clicked();
    void on_generatorButton_clicked();


    void on_udpButton_clicked();

    void on_pregled_clicked();

    void on_actionTrenutni_korisnik_triggered();

private:
    Ui::MainWindow *ui;
    void SaveSettings();
    void LoadSettings();
    QString username;

};



#endif // MAINWINDOW_H
