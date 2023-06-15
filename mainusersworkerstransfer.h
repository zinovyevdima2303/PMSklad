#ifndef MAINUSERSWORKERSTRANSFER_H
#define MAINUSERSWORKERSTRANSFER_H

#include <QSqlQueryModel>
#include <QMainWindow>
#include <QPainter>
#include <QPdfWriter>
#include <QTextDocument>
#include <QFileDialog>
#include <QTableWidget>
#include "qsqlerror.h"
#include "qsqlquery.h"

#include <mainusersworkerstransferadd.h>
#include <mainusersworkerstransfermodify.h>

namespace Ui {
class MainUsersWorkersTransfer;
}

class MainUsersWorkersTransfer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainUsersWorkersTransfer(QWidget *parent = nullptr);
    ~MainUsersWorkersTransfer();

private:
    QString m_idusers, FIO;

signals:
    void MainWorkersTransfer();
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

private:
    Ui::MainUsersWorkersTransfer *ui;
    QSqlQueryModel *TBST;
    MainUsersWorkersTransferAdd* MUWTAdd;
    MainUsersWorkersTransferModify* MUWTM;
};
#endif // MAINUSERSWORKERSTRANSFER_H
