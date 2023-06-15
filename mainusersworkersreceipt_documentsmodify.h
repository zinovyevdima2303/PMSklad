#ifndef MAINUSERSWORKERSRECEIPT_DOCUMENTSMODIFY_H
#define MAINUSERSWORKERSRECEIPT_DOCUMENTSMODIFY_H

#include <QWidget>
#include "qsqlquery.h"

namespace Ui {
class MainUsersWorkersReceipt_documentsModify;
}

class MainUsersWorkersReceipt_documentsModify : public QWidget
{
    Q_OBJECT

public:
    explicit MainUsersWorkersReceipt_documentsModify(QWidget *parent = nullptr);
    ~MainUsersWorkersReceipt_documentsModify();

signals:
    void refresh_table();

private slots:
    void obr_sendIDNakl(int);

    void on_pbEdit_clicked();

    void on_pbClose_clicked();

private:
    Ui::MainUsersWorkersReceipt_documentsModify *ui;
};

#endif // MAINUSERSWORKERSRECEIPT_DOCUMENTSMODIFY_H
