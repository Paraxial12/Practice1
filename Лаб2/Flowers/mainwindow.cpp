#include "mainwindow.h"
#include "build/Desktop_Qt_6_7_1_MinGW_64_bit-Debug/ui_mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./_flowers.db");
    if(db.open()){
        qDebug("opened");
    }
    else{
        qDebug("not opened");
    }
    query = new QSqlQuery(db);

    query->exec("CREATE TABLE flowers (name TEXT, sort TEXT, price REAL);");
    model = new QSqlTableModel(this, db);
    model->setTable("flowers");
    model->select();
    ui->tableView->setModel(model);

    query->exec("CREATE TABLE compositions (name TEXT, flower TEXT, sort TEXT, amount INTEGER);");
    model2 = new QSqlTableModel(this, db);
    model2->setTable("compositions");
    model2->select();
    ui->tableView_2->setModel(model2);

    query->exec("CREATE TABLE orders (accept_date TEXT, order_date TEXT, composition TEXT, amount INTEGER, customer TEXT);");
    model3 = new QSqlTableModel(this, db);
    model3->setTable("orders");
    model3->select();
    ui->tableView_3->setModel(model3);

    query->exec("SELECT o.accept_date, o.order_date, o.composition, o.amount, o.customer, c.sort, c.amount, f.price FROM orders AS o JOIN compositions AS c ON o.composition = c.name JOIN flowers AS f ON c.flower = f.name;");

    browser = ui->textBrowser;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    model->insertRow(model->rowCount());
}

void MainWindow::on_pushButton_2_clicked()
{
    model->removeRow(row1);
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    row1 = index.row();
}


void MainWindow::on_pushButton_4_clicked()
{
    model2->insertRow(model2->rowCount());
}

void MainWindow::on_pushButton_3_clicked()
{
    model2->removeRow(row2);
}

void MainWindow::on_tableView_2_clicked(const QModelIndex &index)
{
    row2 = index.row();
}


void MainWindow::on_pushButton_5_clicked()
{
    model3->insertRow(model3->rowCount());
}

void MainWindow::on_pushButton_6_clicked()
{
    model3->removeRow(row3);
}

void MainWindow::on_tableView_3_clicked(const QModelIndex &index)
{
    row3 = index.row();
}


void MainWindow::on_pushButton_7_clicked()
{
    browser->clear();
    QString queryString = "SELECT SUM(f.price * c.amount * o.amount) AS total_cost "
                          "FROM orders AS o "
                          "JOIN compositions AS c ON o.composition = c.name "
                          "JOIN flowers AS f ON c.flower = f.name;";
    if (query->exec(queryString)) {
        if (query->next()) {
            QSqlRecord record = query->record();
            double totalCost = record.value("total_cost").toDouble();
            QString str = QString::number(totalCost, 'f', 2);
            browser->append(str);
            //qDebug() << "Total cost of orders:" << totalCost;
        }
    }
}

