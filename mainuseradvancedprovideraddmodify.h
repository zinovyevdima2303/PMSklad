#ifndef MAINUSERADVANCEDPROVIDERADDMODIFY_H
#define MAINUSERADVANCEDPROVIDERADDMODIFY_H

#include <QWidget>
#include <QSqlQuery>

namespace Ui {
class mainuseradminmodify;
}

class mainuseradminmodify : public QWidget
{
    Q_OBJECT

public:
    explicit mainuseradminmodify(QWidget *parent = nullptr);
    ~mainuseradminmodify();

signals:
    void refresh_table();

private slots:
    void obr_sendID(int);

private slots:
    void on_btnEdit_clicked();

    void on_btnClose_clicked();

private:
    Ui::mainuseradminmodify *ui;
};

#endif // MAINUSERADVANCEDPROVIDERADDMODIFY_H
