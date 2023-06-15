#ifndef MAINUSERSWORKERSINVENTORYADD_H
#define MAINUSERSWORKERSINVENTORYADD_H

#include <QWidget>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDate>
#include "qsqlerror.h"

namespace Ui {
class MainUsersWorkersInventoryAdd;
}

class MainUsersWorkersInventoryAdd : public QWidget
{
    Q_OBJECT

public:
    explicit MainUsersWorkersInventoryAdd(QWidget *parent = nullptr);
    ~MainUsersWorkersInventoryAdd();

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
    Ui::MainUsersWorkersInventoryAdd *ui;
};

#endif // MAINUSERSWORKERSINVENTORYADD_H
