#ifndef MAINUSERSWORKERSRECEIPT_DOCUMENTSADD_H
#define MAINUSERSWORKERSRECEIPT_DOCUMENTSADD_H

#include <QWidget>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDate>

namespace Ui {
class MainUsersWorkersReceipt_documentsAdd;
}

class MainUsersWorkersReceipt_documentsAdd : public QWidget
{
    Q_OBJECT

public:
    explicit MainUsersWorkersReceipt_documentsAdd(QWidget *parent = nullptr);
    ~MainUsersWorkersReceipt_documentsAdd();

private:
    QString m_idusers,idCount, idSkladProduct, idProduct;

signals:
    void refresh_table();

public slots:
    void recieveID(QString ID);
    void onComboBoxIndexChanged(int index);
    void toggleComboBox();

private slots:
    void on_pbAdd_clicked();

    void on_pbClaer_clicked();

    void on_pbClose_clicked();

private:
    Ui::MainUsersWorkersReceipt_documentsAdd *ui;
};

#endif // MAINUSERSWORKERSRECEIPT_DOCUMENTSADD_H
