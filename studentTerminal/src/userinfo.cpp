#include "studentTerminal/headfile/userinfo.h"
#include "ui_userinfo.h"

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

userInfo::~userInfo()
{
    delete ui;
}
