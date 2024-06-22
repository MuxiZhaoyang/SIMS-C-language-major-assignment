/**
 * @file main.cpp
 * @brief 包含主函数，驱动程序的运行
 */

#include "mainwindow.h"
#include "m_mainwindow.h"
#include "login.h"
#include "smsdatabase.h"

#include <QApplication>
#include <QTimer>

/**
 * @brief 主函数
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    smsDatabase GDB;                //全局数据库访问点

    Login *LoginWindow = new Login(GDB.getConnection());  //创建登录界面

    // 只调用一次exec()等待结果
    const int loginResult = LoginWindow->exec();

    if (loginResult == QDialog::Accepted)
    {
        QString loginUser = LoginWindow->loginUser; // 获取登录用户角色

        if (loginUser == "Student")
        {
            delete LoginWindow; // 正确地删除对象

            MainWindow w(nullptr, GDB.getConnection());       //显示主窗口
            w.showMaximized();

            return a.exec();    //应用程序正常运行
        }
        else if (loginUser == "Manager")
        {
            delete LoginWindow; // 正确地删除对象

            M_MainWindow w(nullptr, GDB.getConnection());       //显示主窗口
            w.showMaximized();

            return a.exec(); //应用程序正常运行
        }
        else
        {
            delete LoginWindow; // 如果登录未被接受，同样需要删除对象
            return 0;
        }
    }
    else
    {
        delete LoginWindow; // 如果登录未被接受，同样需要删除对象
        return 0;
    }

}
