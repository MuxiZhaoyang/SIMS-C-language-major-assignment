#ifndef LOGIN_H
#define LOGIN_H

/***********************************************
 * 登录界面模块
 **********************************************/

#include <QDialog>
#include <QMouseEvent>


#include "smsdatabase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Login;
}
QT_END_NAMESPACE

class Login : public QDialog
{
    Q_OBJECT


private:
    bool    __moving=false;     //表示窗口是否在鼠标操作下移动
    QPoint  __lastPos;          //上一次的鼠标位置

    QString __user;             //用户名
    QString __pswd;             //密码

    int __tryCount=0;           //试错次数

    void    readSettings();     //从注册表读取设置
    void    writeSettings();    //从注册表写入设置
    QString encrypt(const QString& str);    //字符串加密

    QSqlDatabase * __GDB;              //全局数据库访问点（内部）


protected:
    //鼠标事件处理函数，用于拖动窗口
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


private slots:
    void on_btnOK_clicked();    //"确定"按钮

public:
    Login(QSqlDatabase * GDB,QWidget *parent = nullptr);
    ~Login();

    QString loginUser = "Student";//登录省份

private:
    Ui::Login *ui;
};
#endif // LOGIN_H
