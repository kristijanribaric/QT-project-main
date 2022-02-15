#include "table.h"
#include "ui_table.h"
#include <QtWidgets>
#include <QtSql>

Table::Table(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Table)
{
    ui->setupUi(this);
    ui->comboBox->addItem(tr("Sve nacionalnosti"));

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("db/ntp_database.db");

    db.open();
    model = new QSqlRelationalTableModel(this);

    model->setTable("employees");
    model->setRelation(6,QSqlRelation("nationality","id", "name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Ime"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prezime"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Početak god."));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Kraj god."));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Preostalo god."));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Nacionalnost"));

    model->select();
    std::unique_ptr<QTableView> view{new QTableView};
    view->setModel(model);
    view->setItemDelegate(new QSqlRelationalDelegate(view.get()));

    ui->tableView->setModel(model);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    ui->tableView->hideColumn(0);

    QSqlQuery query;
    query.prepare("select * from nationality");

    if(!query.exec()){
        QMessageBox::warning(this,tr("Konekcija"),tr("Upit nije uspješno izvršen!"));

    }
    else {

        while(query.next()){
            QString nameDB = query.value(1).toString();
            nationalities.append(nameDB);
            ui->comboBox->addItem(nameDB);



        }
    }


}

Table::~Table()
{
    delete ui;
}


void Table::on_descButton_clicked()
{
    QSortFilterProxyModel *sort_filter = new QSortFilterProxyModel(this);
    sort_filter->setSourceModel(model);
    sort_filter->sort (2, Qt::DescendingOrder);
    ui->tableView->setModel(sort_filter);
}


void Table::on_ascButton_clicked()
{
    QSortFilterProxyModel *sort_filter = new QSortFilterProxyModel(this);
    sort_filter->setSourceModel(model);
    sort_filter->sort (2,Qt::AscendingOrder);
    ui->tableView->setModel(sort_filter);
}


void Table::on_filterButton_clicked()
{
    model->setFilter(" nationality == 3");
    ui->tableView->setModel(model);
}




void Table::on_comboBox_textActivated(const QString &arg1)
{
    qDebug() << arg1;
    if(arg1 == tr("Sve nacionalnosti")){
        model->setFilter(" nationality ");
        ui->tableView->setModel(model);
    }

    for(int i = 0; i < nationalities.size(); i++) {

        if(arg1 == nationalities.at(i)){
            model->setFilter(QString("nationality == %1").arg(i + 1));
            ui->tableView->setModel(model);
        }
    }
}

