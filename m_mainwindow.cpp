/**
 * @file m_mainwindow.cpp
 * @brief 这个文件包含了 `M_MainWindow` 类的实现，作为管理员端的主界面
 */

#include "m_mainwindow.h"
#include "ui_m_mainwindow.h"

/**
 * @brief 构造函数，进行界面初始化
 * 
 * @param GDB 全局数据库连接
 * @param parent 父对象
 */
M_MainWindow::M_MainWindow(QWidget *parent,QSqlDatabase * GDB)
    : QMainWindow(parent)
    , __GDB(GDB)
    , ui(new Ui::M_MainWindow)
{
    ui->setupUi(this);

    initAll();          //总初始化函数

}

/**
 * @brief 析构函数
 * 
 */
M_MainWindow::~M_MainWindow()
{
    delete ui;
}

/**
 * @brief 总初始化函数
 * 
 * 包含所有初始化内容
 */
void M_MainWindow::initAll()
{//总初始化函数

    //状态栏显示北京时间模块
    timer = new QTimer;
    timer->setInterval(1000);
    timer->setSingleShot(false);
    timer->setParent(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(do_timerout()));      //槽链接

    timer->start();

    labTime = new QLabel(this);
    labTime->setMinimumWidth(150);
    ui->statusbar->addPermanentWidget(labTime);         //添加组件

    //状态栏显示山东大学图标
    sduIcon = new QLabel(this);
    QPixmap pic;
    pic.load(":/icon/images/sdu.png");
    pic = pic.scaled(200, 25, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    sduIcon->setPixmap(pic);
    ui->statusbar->addPermanentWidget(sduIcon);

    //设置工具栏QAction类选择互斥
    A1=new QActionGroup(this);
    ui->a1->setActionGroup(A1);
    ui->a2->setActionGroup(A1);
    ui->a3->setActionGroup(A1);
    ui->a4->setActionGroup(A1);
    ui->a5->setActionGroup(A1);
    ui->a6->setActionGroup(A1);
    ui->a8->setActionGroup(A1);

    //初始化堆叠窗口
    windows = new QStackedWidget(this);
    windows->setVisible(false);
    this->setCentralWidget(windows);

    a1_studentManagement = new studentManagement(this,__GDB);
    a2_scoreManagement = new scoreManagement(this,__GDB);
    a3_selectionManagement = new selectionManagement(this,__GDB);
    a4_courseManagement = new courseManagement(this,__GDB);
    a5_teacherManagement = new teacherManagement(this,__GDB);
    a6_classManagement = new classManagement(this,__GDB);
    a8_MchangePassword = new MchangePassword(this,__GDB);

    windows->addWidget(a1_studentManagement);
    windows->addWidget(a2_scoreManagement);
    windows->addWidget(a3_selectionManagement);
    windows->addWidget(a4_courseManagement);
    windows->addWidget(a5_teacherManagement);
    windows->addWidget(a6_classManagement);
    windows->addWidget(a8_MchangePassword);

    windows->setCurrentIndex(0);
    windows->setVisible(true);
}

/**
 * @brief 退出时询问，生成询问对话框
 * 
 * @param event 全局事件
 */
void M_MainWindow::closeEvent(QCloseEvent *event)
{//退出时询问，生成询问对话框
    QMessageBox::StandardButton result;
    result = QMessageBox::question(this,"退出","是否要退出学生管理系统？您所有未保存的修改将丢失。",
                                   QMessageBox::StandardButtons(QMessageBox::Yes|QMessageBox::No),QMessageBox::No);
    if(result == QMessageBox::Yes)event->accept();
    else event->ignore();
}

/**
 * @brief 定时器刷新时间槽函数
 * 
 */
void M_MainWindow::do_timerout()
{//定时器刷新时间函数
    QTime curtime = QTime::currentTime();
    QString str = curtime.toString();
    QString tim = QString("北京时间： %1").arg(str);
    labTime->setText(tim);
}

/**
 * @brief 界面刷新槽函数
 * 
 */
void M_MainWindow::on_a1_triggered()
{
    windows->setCurrentIndex(0);
}

/**
 * @brief 界面刷新槽函数
 * 
 */
void M_MainWindow::on_a2_triggered()
{
    windows->setCurrentIndex(1);
}

/**
 * @brief 界面刷新槽函数
 * 
 */
void M_MainWindow::on_a3_triggered()
{
    windows->setCurrentIndex(2);
}

/**
 * @brief 界面刷新槽函数
 * 
 */
void M_MainWindow::on_a4_triggered()
{
    windows->setCurrentIndex(3);
}

/**
 * @brief 界面刷新槽函数
 * 
 */
void M_MainWindow::on_a5_triggered()
{
    windows->setCurrentIndex(4);
}

/**
 * @brief 界面刷新槽函数
 * 
 */
void M_MainWindow::on_a6_triggered()
{
    windows->setCurrentIndex(5);
}

/**
 * @brief 界面刷新槽函数
 * 
 */
void M_MainWindow::on_a8_triggered()
{
    windows->setCurrentIndex(6);
}

