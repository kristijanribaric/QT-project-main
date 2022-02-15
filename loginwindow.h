#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QtWidgets>
#include <QtXml>
#include "mainwindow.h"
#include "threadworker.h"
#include <QFile>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    ThreadWorker_Settings *threadSettings;
    ThreadWorker_Json *threadJson;
    ThreadWorker_Xml *threadXml;

private slots:
    void on_login_clicked();

    void on_last_login_clicked();

    void on_last_login2_clicked();

private:
    Ui::LoginWindow *ui;
    MainWindow *main;

    void SaveSettings();
    void LoadSettings();
    int succes = 0;


};

#endif // LOGINWINDOW_H
