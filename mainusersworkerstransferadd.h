#ifndef MAINUSERSWORKERSTRANSFERADD_H
#define MAINUSERSWORKERSTRANSFERADD_H

#include <QWidget>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDate>
#include "qsqlerror.h"

namespace Ui {
class MainUsersWorkersTransferAdd;
}

class MainUsersWorkersTransferAdd : public QWidget
{
    Q_OBJECT

public:
    explicit MainUsersWorkersTransferAdd(QWidget *parent = nullptr);
    ~MainUsersWorkersTransferAdd();

private:
    QString m_idusers, idSkladProduct;


signals:
    void refresh_table();

public slots:
    void recieveID(QString ID);
    void onComboBoxIndexChanged(int index);

private slots:
    void on_pbAdd_clicked();

    void on_pbClear_clicked();

    void on_pbClose_clicked();

private:
    Ui::MainUsersWorkersTransferAdd *ui;
};

#endif // MAINUSERSWORKERSTRANSFERADD_H
