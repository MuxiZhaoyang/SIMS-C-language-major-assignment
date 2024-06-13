#ifndef MCHANGEPASSWORD_H
#define MCHANGEPASSWORD_H

/***********************************************
 * 修改密码模块
 **********************************************/

#include <QWidget>

#include "smsdatabase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MchangePassword;
}
QT_END_NAMESPACE

class MchangePassword : public QWidget
{
    Q_OBJECT

private:
    QString __pswd;             //密码

    int __tryCount=0;           //试错次数

    QString encrypt(const QString& str);    //字符串加密

    QSqlDatabase * __GDB;              //全局数据库访问点（内部）

public:
    MchangePassword(QWidget *parent = nullptr,QSqlDatabase * GDB = nullptr);
    ~MchangePassword();

    bool test_password(const QString & pswd);       //测试密码安全性

private slots:
    void on_btnOK_clicked();

    void on_btnCancel_clicked();

private:
    Ui::MchangePassword *ui;
};
#endif // MCHANGEPASSWORD_H
