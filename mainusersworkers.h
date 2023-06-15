#ifndef MAINUSERSWORKERS_H
#define MAINUSERSWORKERS_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <mainusersworkersreceipt_documents.h>
#include <mainusersworkerstransfer.h>
#include <mainusersworkersinventory.h>
#include <mainusersworkersreturn.h>
#include "qsqlerror.h"
#include <QSortFilterProxyModel>
namespace Ui {
class MainUsersWorkers;
}

class MainUsersWorkers : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainUsersWorkers(QWidget *parent = nullptr);
    ~MainUsersWorkers();

private:
    QString m_idusers, FIO;

signals:
    void MainWorkers();
    void sendID(QString ID);

public slots:
    void recieveID(QString ID);
    void obr_ref_tab();

private slots:
    void on_btnRefresh_2_clicked();

    void on_btnClose_2_clicked();

    void on_pbReceipt_documents_clicked();

    void on_pbTransfer_invoices_clicked();

    void on_pbReturn_documents_clicked();

    void on_pbInventory_clicked();

    void on_pbPrint_clicked();

    void on_pbSearchLocation_clicked();

    void on_pbSearchProduct_clicked();

private:
    Ui::MainUsersWorkers *ui;
    QSqlQueryModel *TBS;
    MainUsersWorkersReceipt_documents *MUWRd;
    MainUsersWorkersTransfer *MUWT;
    MainUsersWorkersInventory *MUWI;
    MainUsersWorkersReturn *MUWR;
};

#endif // MAINUSERSWORKERS_H
