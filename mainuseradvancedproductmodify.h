#ifndef MAINUSERADVANCEDPRODUCTMODIFY_H
#define MAINUSERADVANCEDPRODUCTMODIFY_H

#include <QWidget>
#include <QSqlQuery>

namespace Ui {
class MainUserAdvancedProductModify;
}

class MainUserAdvancedProductModify : public QWidget
{
    Q_OBJECT

public:
    explicit MainUserAdvancedProductModify(QWidget *parent = nullptr);
    ~MainUserAdvancedProductModify();

signals:
    void refresh_table();

private slots:
    void obr_sendID(int);

private slots:
    void on_btnEdit_clicked();

    void on_btnClose_clicked();

private:
    Ui::MainUserAdvancedProductModify *ui;
};

#endif // MAINUSERADVANCEDPRODUCTMODIFY_H
