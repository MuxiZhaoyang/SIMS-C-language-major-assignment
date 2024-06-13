#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/***********************************************
 * 学生界面模块
 **********************************************/

#include "qlabel.h"
#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QPaintEvent>
#include <QMessageBox>
#include <QActionGroup>
#include <QStackedWidget>

#include "studentTerminal/headfile/userinfo.h"
#include "studentTerminal/headfile/scorequery.h"
#include "studentTerminal/headfile/courseselection.h"
#include "studentTerminal/headfile/courseoverview.h"
#include "studentTerminal/headfile/courseinformation.h"
#include "studentTerminal/headfile/changepassword.h"
#include "smsdatabase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    void initAll();            //总初始化函数,用于创建代码组件

    QTimer *timer;              //定时器用于刷新时间
    QLabel *sduIcon;
    QLabel *labTime;            //用于创建状态栏显示时间

    QActionGroup *A1;           //用于实现互斥选择

    QStackedWidget *windows;    //用于显示屏幕

    userInfo *a1_userInfo;                  //个人信息窗口
    scoreQuery *a2_scoreQuery;              //成绩查询窗口
    courseOverview *a4_courseOverview;       //选课浏览窗口
    courseInformation *a5_courseInformation; //课程信息窗口
    changePassword *a6_changePassword;       //修改密码窗口

    QSqlDatabase * __GDB;                    //全局数据库接入点

protected:
    void closeEvent(QCloseEvent *event);    //退出时询问

public slots:
    void do_timerout();         //用于刷新时间

public:
    MainWindow(QWidget *parent = nullptr,QSqlDatabase * GDB = nullptr);
    ~MainWindow();

private slots:
    void on_a2_triggered();

    void on_a1_triggered();

    void on_a4_triggered();

    void on_a5_triggered();

    void on_a6_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
