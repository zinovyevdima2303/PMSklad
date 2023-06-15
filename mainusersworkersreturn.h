#ifndef MAINUSERSWORKERSRETURN_H
#define MAINUSERSWORKERSRETURN_H
#include "mainusersworkersreturnadd.h"
#include "mainusersworkersreturnmodify.h"

#include <QSqlQueryModel>
#include <QMainWindow>
#include <QPainter>
#include <QPdfWriter>
#include <QTextDocument>
#include <QFileDialog>
#include <QTableWidget>
#include "qsqlerror.h"
#include "qsqlquery.h"

namespace Ui {
class MainUsersWorkersReturn;
}

class MainUsersWorkersReturn : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainUsersWorkersReturn(QWidget *parent = nullptr);
    ~MainUsersWorkersReturn();

private:
    QString m_idusers, FIO;
signals:
    void MainWorkersReturn();
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
    Ui::MainUsersWorkersReturn *ui;
    QSqlQueryModel *TBSR;
    MainUsersWorkersReturnAdd* MUWRAdd;
    MainUsersWorkersReturnModify* MUWRM;
};
#endif // MAINUSERSWORKERSRETURN_H
