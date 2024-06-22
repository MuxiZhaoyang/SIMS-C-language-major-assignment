/**
 * @file userinfo.cpp
 * @brief 这个文件包含了 `userInfo` 类的实现，用于显示学生个人信息
 */

#include "studentTerminal/headfile/userinfo.h"
#include "ui_userinfo.h"

/**
 * @brief 构造函数，用于初始化用户信息界面
 *
 * @param parent 父窗口指针
 * @param GDB 数据库指针
 */
userInfo::userInfo(QWidget *parent,QSqlDatabase * GDB)
    : QWidget(parent)
    , __GDB(GDB)
    , ui(new Ui::userInfo)
{
    ui->setupUi(this);

    QSettings set;      //从注册表里读取用户名

    __student_ID = set.value("Username").toString();

    QSqlQuery db(*__GDB);                           //搜索数据库
    db.prepare("SELECT student.studentID, student.gender, student.birthday, student.province, student.photo, student.name AS studentName, class.name AS className, major.name AS majorName, school.name AS schoolName "
               "FROM student "
               "INNER JOIN class ON student.classID = class.classID "
               "INNER JOIN major ON class.majorID = major.majorID "
               "INNER JOIN school ON major.schoolID = school.schoolID "
               "WHERE student.studentID = :user ");
    db.bindValue(":user",__student_ID);         //绑定占位符
    db.exec();                          //搜索指令运行

    if(db.first())
    {//写入页面
        ui->studID->setText(db.value("student.studentID").toString());
        ui->name->setText(db.value("studentName").toString());
        ui->birth->setText(db.value("student.birthday").toString());
        ui->class_2->setText(db.value("className").toString());
        ui->school->setText(db.value("schoolName").toString());
        ui->province->setText(db.value("student.province").toString());
        ui->gender->setText(db.value("student.gender").toString());
        ui->majoy->setText(db.value("majorName").toString());
        QByteArray data=db.value("student.photo").toByteArray();
        QPixmap pic;
        pic.loadFromData(data);
        QSize labelSize = ui->photo->size();
        pic = pic.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->photo->setPixmap(pic);
    }
}

/**
 * @brief 析构函数，释放资源
 */
userInfo::~userInfo()
{
    delete ui;
}
