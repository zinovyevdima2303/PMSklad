#ifndef MAINUSERSWORKERSINVENTORY_H
#define MAINUSERSWORKERSINVENTORY_H

#include <QSqlQueryModel>
#include <QMainWindow>
#include <QPainter>
#include <QPdfWriter>
#include <QTextDocument>
#include <QFileDialog>
#include <QTableWidget>
#include "qsqlerror.h"
#include "qsqlquery.h"

#include"mainusersworkersinventoryadd.h"
#include"mainusersworkersinventorymodify.h"
#include"mainusersworkersinventoryprint.h"

namespace Ui {
class MainUsersWorkersInventory;
}

class MainUsersWorkersInventory : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainUsersWorkersInventory(QWidget *parent = nullptr);
    ~MainUsersWorkersInventory();

private:
    QString m_idusers, FIO;

signals:
    void MainWorkersInventory();
    void sendID(QString ID);
    void sendIDNakl(int);

public slots:
    void recieveID(QString ID);
    void obr_ref_tab();

signals:
    void refresh_table();

private slots:
    void on_btnRefresh_clicked();

    void on_btnClose_clicked();

    void on_pbAdd_clicked();

    void on_pbEdit_clicked();

    void on_pbPrint_clicked();

    void on_pbPrintFilter_clicked();

private:
    Ui::MainUsersWorkersInventory *ui;
    QSqlQueryModel *TBSI;
    MainUsersWorkersInventoryAdd* MUWIAdd;
    MainUsersWorkersInventoryModify* MUWIM;
    MainUsersWorkersInventoryPrint* MUWIP;
};

#endif // MAINUSERSWORKERSINVENTORY_H
