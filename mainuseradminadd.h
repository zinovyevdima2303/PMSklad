#ifndef MAINUSERADMINADD_H
#define MAINUSERADMINADD_H

#include <QWidget>
#include <QMessageBox>
#include <QSqlQuery>
#include <QComboBox>
#include <QLineEdit>
#include <QKeyEvent>
#include <QRegularExpression>

namespace Ui {
class mainuseradminadd;
}

class mainuseradminadd : public QWidget
{
    Q_OBJECT

public:
    explicit mainuseradminadd(QWidget *parent = nullptr);
    ~mainuseradminadd();

private slots:
    void on_btnAdd_clicked();

    void on_btnClose_clicked();

    void on_btnRefresh_clicked();

    void on_pbLookpassword_clicked();

signals:
    void refresh_table();

private:
    Ui::mainuseradminadd *ui;
    bool eventFilter(QObject *object, QEvent *event) override;

};

#endif // MAINUSERADMINADD_H
