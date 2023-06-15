#ifndef MAINUSERSWORKERSRECEIPT_DOCUMENTS_H
#define MAINUSERSWORKERSRECEIPT_DOCUMENTS_H

#include <QSqlQueryModel>
#include <QMainWindow>
#include <QPainter>
#include <QPdfWriter>
#include <QTextDocument>
#include <QFileDialog>

#include <mainusersworkersreceipt_documentsadd.h>
#include <mainusersworkersreceipt_documentsmodify.h>

namespace Ui {
class MainUsersWorkersReceipt_documents;
}

class MainUsersWorkersReceipt_documents : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainUsersWorkersReceipt_documents(QWidget *parent = nullptr);
    ~MainUsersWorkersReceipt_documents();

private:
    QString m_idusers, FIO;

signals:
    void MainWorkersReceipt_documents();
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
    Ui::MainUsersWorkersReceipt_documents *ui;
    QSqlQueryModel *TBSRd;
    MainUsersWorkersReceipt_documentsAdd* MUWRdAdd;
    MainUsersWorkersReceipt_documentsModify* MUWRdM;
};

#endif // MAINUSERSWORKERSRECEIPT_DOCUMENTS_H
