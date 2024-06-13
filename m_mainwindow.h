#ifndef M_MAINWINDOW_H
#define M_MAINWINDOW_H

/***********************************************
 * 管理员界面模块
 **********************************************/

#include "qlabel.h"
#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QPaintEvent>
#include <QMessageBox>
#include <QActionGroup>
#include <QStackedWidget>

#include "managerTerminal/headfile/classmanagement.h"
#include "managerTerminal/headfile/coursemanagement.h"
#include "managerTerminal/headfile/scoremanagement.h"
#include "managerTerminal/headfile/selectionmanagement.h"
#include "managerTerminal/headfile/teachermanagement.h"
#include "managerTerminal/headfile/studentmanagement.h"
#include "managerTerminal/headfile/Mchangepassword.h"
#include "smsdatabase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class M_MainWindow;
}
QT_END_NAMESPACE

class M_MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    void initAll();            //总初始化函数,用于创建代码组件

    QTimer *timer;              //定时器用于刷新时间
    QLabel *sduIcon;
    QLabel *labTime;            //用于创建状态栏显示时间

    QActionGroup *A1;           //用于实现互斥选择

    QStackedWidget *windows;    //用于显示屏幕

    studentManagement *a1_studentManagement;                  //学生管理窗口
    scoreManagement *a2_scoreManagement;              //成绩管理窗口
    selectionManagement *a3_selectionManagement;     //学生选课窗口
    courseManagement *a4_courseManagement;       //课程管理窗口
    teacherManagement *a5_teacherManagement;    //教师管理窗口
    classManagement *a6_classManagement;       //班级管理窗口
    MchangePassword *a8_MchangePassword;        //修改密码窗口

    QSqlDatabase * __GDB;                    //全局数据库接入点

protected:
    void closeEvent(QCloseEvent *event);    //退出时询问

public slots:
    void do_timerout();         //用于刷新时间

public:
    M_MainWindow(QWidget *parent = nullptr,QSqlDatabase * GDB = nullptr);
    ~M_MainWindow();

private slots:
    void on_a1_triggered();

    void on_a2_triggered();

    void on_a3_triggered();

    void on_a4_triggered();

    void on_a5_triggered();

    void on_a6_triggered();

    void on_a8_triggered();

private:
    Ui::M_MainWindow *ui;
};
#endif // M_MAINWINDOW_H
