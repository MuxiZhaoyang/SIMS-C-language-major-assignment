#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

/***********************************************
 * 修改密码模块
 **********************************************/

#include <QWidget>

#include "smsdatabase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class changePassword;
}
QT_END_NAMESPACE

class changePassword : public QWidget
{
    Q_OBJECT

private:
    QString __user;             //用户名
    QString __pswd;             //密码

    int __tryCount=0;           //试错次数

    void    readSettings();     //从注册表读取设置
    void    writeSettings();    //从注册表写入设置
    QString encrypt(const QString& str);    //字符串加密

    QSqlDatabase * __GDB;              //全局数据库访问点（内部）

public:
    changePassword(QWidget *parent = nullptr,QSqlDatabase * GDB = nullptr);
    ~changePassword();

    bool test_password(const QString & pswd);       //测试密码安全性

private slots:
    void on_btnOK_clicked();

    void on_btnCancel_clicked();

private:
    Ui::changePassword *ui;
};
#endif // CHANGEPASSWORD_H
