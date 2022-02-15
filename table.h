#ifndef TABLE_H
#define TABLE_H

#include <QDialog>
#include <QtGui>
#include <QtCore>
#include <QtSql>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>



namespace Ui {
class Table;
}

class Table : public QDialog
{
    Q_OBJECT

public:
    explicit Table(QWidget *parent = nullptr);
    ~Table();


private slots:
    void on_descButton_clicked();

    void on_ascButton_clicked();

    void on_filterButton_clicked();

    void on_comboBox_textActivated(const QString &arg1);

private:
    Ui::Table *ui;
    QSqlDatabase db;
    QSqlRelationalTableModel *model;
    QStringList nationalities;
};

#endif // TABLE_H
