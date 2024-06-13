#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent,QSqlDatabase * GDB)
    : QMainWindow(parent)
    , __GDB(GDB)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initAll();          //总初始化函数
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initAll()
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
    ui->a4->setActionGroup(A1);
    ui->a5->setActionGroup(A1);
    ui->a6->setActionGroup(A1);

    //初始化堆叠窗口
    windows = new QStackedWidget(this);
    windows->setVisible(false);
    this->setCentralWidget(windows);

    a1_userInfo = new userInfo(this,__GDB);
    a2_scoreQuery = new scoreQuery(this,__GDB);
    a4_courseOverview = new courseOverview(this,__GDB);
    a5_courseInformation = new courseInformation(this,__GDB);
    a6_changePassword = new changePassword(this,__GDB);

    windows->addWidget(a1_userInfo);
    windows->addWidget(a2_scoreQuery);
    windows->addWidget(a4_courseOverview);
    windows->addWidget(a5_courseInformation);
    windows->addWidget(a6_changePassword);

    windows->setCurrentIndex(0);
    windows->setVisible(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{//退出时询问，生成询问对话框
    QMessageBox::StandardButton result;
    result = QMessageBox::question(this,"退出","是否要退出学生管理系统？您所有未保存的修改将丢失。",
                QMessageBox::StandardButtons(QMessageBox::Yes|QMessageBox::No),QMessageBox::No);
    if(result == QMessageBox::Yes)event->accept();
    else event->ignore();
}

void MainWindow::do_timerout()
{//定时器刷新时间函数
    QTime curtime = QTime::currentTime();
    QString str = curtime.toString();
    QString tim = QString("北京时间： %1").arg(str);
    labTime->setText(tim);
}

void MainWindow::on_a1_triggered()
{
    windows->setCurrentIndex(0);
}

void MainWindow::on_a2_triggered()
{
    windows->setCurrentIndex(1);
}


void MainWindow::on_a4_triggered()
{
    windows->setCurrentIndex(2);
}


void MainWindow::on_a5_triggered()
{
    windows->setCurrentIndex(3);
}


void MainWindow::on_a6_triggered()
{
    windows->setCurrentIndex(4);
}

