#include "loginwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QInputDialog>
#include <QMainWindow>
#include <QtCore>
#include <QFile>
#include <QtXml>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    QStringList languages;
    languages << "Croatian" << "English" << "French";

    QString lang = QInputDialog::getItem(NULL, "Odaberi jezik","Jezik",languages);
    if(lang == "English"){
        translator.load(":/english.qm");
    }else if (lang == "French") {
        translator.load(":/french.qm");
    }

    if( lang != "Croatian"){
        a.installTranslator(&translator);
    }


    LoginWindow w;
    w.show();
    return a.exec();
}
