#include "studentTerminal/headfile/courseoverview.h"
#include "ui_courseoverview.h"

courseOverview::courseOverview(QWidget *parent,QSqlDatabase * GDB)
    : QWidget(parent)
    , __GDB(GDB)
    , ui(new Ui::courseOverview)
{
    ui->setupUi(this);

    QSettings set;
    user = set.value("Username").toString();
    //创建数据模型，打开数据表
    tabModel=new QSqlQueryModel(this);	//数据模型

    //初始化复选框
    sql = "SELECT settingContent FROM setting WHERE settingName = 'addTerm' ORDER BY settingContent DESC ";
    QSqlQuery ser(sql,*__GDB);
    QStringList termList;       //实用字符串链表组织学期
    while(ser.next())termList<<ser.value("settingContent").toString();
    ui->termSelection->addItems(termList);      //获取可用学期加入复选框

    //SQL搜索语句
    sql = QString("SELECT term, course.name AS courseName, course.type AS courseType, course.credit AS credit, teacher.name AS teacherName, course.classTime AS classTime ,course.place AS place, school.name AS schoolName "
                  "FROM score "
                  "INNER JOIN course ON score.courseID = course.courseID "
                  "INNER JOIN teacher ON course.teacherID = teacher.teacherID "
                  "INNER JOIN school ON course.schoolID = school.schoolID "
                  "WHERE score.studentID = %1 " ).arg(user);        //保存基础语句用于刷新搜索
    QString str =sql + QString("AND score.term = '%1' ").arg(ui->termSelection->currentText());

    tabModel->setQuery(str,*__GDB);			//设置数据表

    //计算学分和
    str = QString("SELECT SUM(course.credit) AS sumCredit FROM score INNER JOIN course ON score.courseID = course.courseID WHERE term = '%1';").arg(ui->termSelection->currentText());
    QSqlQuery ser2(str,*__GDB);
    ser2.exec();
    ser2.first();
    int sumCredit= ser2.value("sumCredit").toInt();
    ui->point->setText(QString("%1").arg(sumCredit));

    if (tabModel->lastError().isValid())
    {//查询数据失败
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n"+tabModel->lastError().text());
        return;
    }

    //设置字段显示标题
    QSqlRecord rec = tabModel->record();        //获取一个空记录，用于获取字段序号
    tabModel->setHeaderData(rec.indexOf("courseName"), Qt::Horizontal,"课程");
    tabModel->setHeaderData(rec.indexOf("courseType"), Qt::Horizontal, "课程类型");
    tabModel->setHeaderData(rec.indexOf("teacherName"),   Qt::Horizontal, "授课教师");
    tabModel->setHeaderData(rec.indexOf("credit"),   Qt::Horizontal, "学分");
    tabModel->setHeaderData(rec.indexOf("classTime"), Qt::Horizontal, "上课时间");
    tabModel->setHeaderData(rec.indexOf("place"), Qt::Horizontal, "地点");
    tabModel->setHeaderData(rec.indexOf("gradePoint"), Qt::Horizontal, "绩点");
    tabModel->setHeaderData(rec.indexOf("term"), Qt::Horizontal, "学期");
    tabModel->setHeaderData(rec.indexOf("schoolName"), Qt::Horizontal, "开设学院");

    //Model/View结构
    ui->tableView->setModel(tabModel);	//设置数据模型

    setClassSchedule(); //加载课程表

    ui->term->setText(ui->termSelection->currentText());    //刷新学期

    ui->radioBtnAscend->setChecked(true);
    ui->radioBtnDescend->setChecked(false);
    ui->radioBtnAscend->setEnabled(false);
    ui->radioBtnDescend->setEnabled(false);
}

courseOverview::~courseOverview()
{
    delete ui;
}

void courseOverview::setClassSchedule()
{//加载课程表
    QSqlRecord rec = tabModel->record();        //获取一个空记录，用于获取字段序号
    int courseName = rec.indexOf("courseName"),classTime = rec.indexOf("classTime"),place = rec.indexOf("place");   //获取字段号
    if(tabModel->rowCount() > 0) {
        for (int row = 0; row < tabModel->rowCount(); ++row) {
            // 获取该行该列的内容
            QStringList parts = tabModel->data(tabModel->index(row, classTime)).toString().split(" ");
            QString _name = tabModel->data(tabModel->index(row, courseName)).toString();
            QString _place = tabModel->data(tabModel->index(row, place)).toString();

            for(QString str : parts)
            {//加载课程表，变色与显示地点
                QGroupBox * d;  //指向组
                QLabel * dt;    //指向标签

                if     (str ==  "星期一第一大节"){d = ui->d11;dt = ui->dt11;}
                else if(str ==  "星期一第二大节"){d = ui->d12;dt = ui->dt12;}
                else if(str ==  "星期一第三大节"){d = ui->d13;dt = ui->dt13;}
                else if(str ==  "星期一第四大节"){d = ui->d14;dt = ui->dt14;}
                else if(str ==  "星期一第五大节"){d = ui->d15;dt = ui->dt15;}

                else if(str ==  "星期二第一大节"){d = ui->d21;dt = ui->dt21;}
                else if(str ==  "星期二第二大节"){d = ui->d22;dt = ui->dt22;}
                else if(str ==  "星期二第三大节"){d = ui->d23;dt = ui->dt23;}
                else if(str ==  "星期二第四大节"){d = ui->d24;dt = ui->dt24;}
                else if(str ==  "星期二第五大节"){d = ui->d25;dt = ui->dt25;}

                else if(str ==  "星期三第一大节"){d = ui->d31;dt = ui->dt31;}
                else if(str ==  "星期三第二大节"){d = ui->d32;dt = ui->dt32;}
                else if(str ==  "星期三第三大节"){d = ui->d33;dt = ui->dt33;}
                else if(str ==  "星期三第四大节"){d = ui->d34;dt = ui->dt34;}
                else if(str ==  "星期三第五大节"){d = ui->d35;dt = ui->dt35;}

                else if(str ==  "星期四第一大节"){d = ui->d41;dt = ui->dt41;}
                else if(str ==  "星期四第二大节"){d = ui->d42;dt = ui->dt42;}
                else if(str ==  "星期四第三大节"){d = ui->d43;dt = ui->dt43;}
                else if(str ==  "星期四第四大节"){d = ui->d44;dt = ui->dt44;}
                else if(str ==  "星期四第五大节"){d = ui->d45;dt = ui->dt45;}

                else if(str ==  "星期五第一大节"){d = ui->d51;dt = ui->dt51;}
                else if(str ==  "星期五第二大节"){d = ui->d52;dt = ui->dt52;}
                else if(str ==  "星期五第三大节"){d = ui->d53;dt = ui->dt53;}
                else if(str ==  "星期五第四大节"){d = ui->d54;dt = ui->dt54;}
                else if(str ==  "星期五第五大节"){d = ui->d55;dt = ui->dt55;}

                else if(str ==  "星期六第一大节"){d = ui->d61;dt = ui->dt61;}
                else if(str ==  "星期六第二大节"){d = ui->d62;dt = ui->dt62;}
                else if(str ==  "星期六第三大节"){d = ui->d63;dt = ui->dt63;}
                else if(str ==  "星期六第四大节"){d = ui->d64;dt = ui->dt64;}
                else if(str ==  "星期六第五大节"){d = ui->d65;dt = ui->dt65;}

                else if(str ==  "星期日第一大节"){d = ui->d71;dt = ui->dt71;}
                else if(str ==  "星期日第二大节"){d = ui->d72;dt = ui->dt72;}
                else if(str ==  "星期日第三大节"){d = ui->d73;dt = ui->dt73;}
                else if(str ==  "星期日第四大节"){d = ui->d74;dt = ui->dt74;}
                else if(str ==  "星期日第五大节"){d = ui->d75;dt = ui->dt75;}

                else break;

                d->setStyleSheet("QGroupBox { background-color: red; }"
                                 "QGroupBox::contents { background-color: red; }"); // 针对内部区域
                dt->setText(_name + "\n" + _place);
            }
        }
    }

}

void courseOverview::initClassSchedule()
{//初始化课程表
    ui->d11->setStyleSheet("");
    ui->d12->setStyleSheet("");
    ui->d13->setStyleSheet("");
    ui->d14->setStyleSheet("");
    ui->d15->setStyleSheet("");

    ui->d21->setStyleSheet("");
    ui->d22->setStyleSheet("");
    ui->d23->setStyleSheet("");
    ui->d24->setStyleSheet("");
    ui->d25->setStyleSheet("");

    ui->d31->setStyleSheet("");
    ui->d32->setStyleSheet("");
    ui->d33->setStyleSheet("");
    ui->d34->setStyleSheet("");
    ui->d35->setStyleSheet("");

    ui->d41->setStyleSheet("");
    ui->d42->setStyleSheet("");
    ui->d43->setStyleSheet("");
    ui->d44->setStyleSheet("");
    ui->d45->setStyleSheet("");

    ui->d51->setStyleSheet("");
    ui->d52->setStyleSheet("");
    ui->d53->setStyleSheet("");
    ui->d54->setStyleSheet("");
    ui->d55->setStyleSheet("");

    ui->d61->setStyleSheet("");
    ui->d62->setStyleSheet("");
    ui->d63->setStyleSheet("");
    ui->d64->setStyleSheet("");
    ui->d65->setStyleSheet("");

    ui->d71->setStyleSheet("");
    ui->d72->setStyleSheet("");
    ui->d73->setStyleSheet("");
    ui->d74->setStyleSheet("");
    ui->d75->setStyleSheet("");

    ui->dt11->setText("");
    ui->dt12->setText("");
    ui->dt13->setText("");
    ui->dt14->setText("");
    ui->dt15->setText("");

    ui->dt21->setText("");
    ui->dt22->setText("");
    ui->dt23->setText("");
    ui->dt24->setText("");
    ui->dt25->setText("");

    ui->dt31->setText("");
    ui->dt32->setText("");
    ui->dt33->setText("");
    ui->dt34->setText("");
    ui->dt35->setText("");

    ui->dt41->setText("");
    ui->dt42->setText("");
    ui->dt43->setText("");
    ui->dt44->setText("");
    ui->dt45->setText("");

    ui->dt51->setText("");
    ui->dt52->setText("");
    ui->dt53->setText("");
    ui->dt54->setText("");
    ui->dt55->setText("");

    ui->dt61->setText("");
    ui->dt62->setText("");
    ui->dt63->setText("");
    ui->dt64->setText("");
    ui->dt65->setText("");

    ui->dt71->setText("");
    ui->dt72->setText("");
    ui->dt73->setText("");
    ui->dt74->setText("");
    ui->dt75->setText("");
}

void courseOverview::on_comboFields_currentTextChanged(const QString &arg1)
{//切换排序方式
    Q_UNUSED(arg1)
    QString str,now;
    str = sql + QString("AND score.term = '%1' ").arg(ui->termSelection->currentText());
    now = ui->comboFields->currentText();
    if(now == "默认排序")
    {
        ui->radioBtnAscend->setChecked(true);
        ui->radioBtnDescend->setChecked(false);
        ui->radioBtnAscend->setEnabled(false);
        ui->radioBtnDescend->setEnabled(false);
    }
    else
    {
        ui->radioBtnAscend->setEnabled(true);
        ui->radioBtnDescend->setEnabled(true);
        if(now == "学分")str += "ORDER BY credit ";
    }

    if(ui->radioBtnAscend->isChecked() && now != "默认排序")str += "ASC ";
    else if(now != "默认排序")str += "DESC ";

    tabModel->setQuery(str,*__GDB);     //二次查询
}


void courseOverview::on_radioBtnAscend_clicked()
{//切换排序方式
    QString str,now;
    now = ui->comboFields->currentText();
    str = sql + QString("AND score.term = '%1' ").arg(ui->termSelection->currentText());
    if(now == "默认排序");
    else if(now == "学分")str += "ORDER BY credit ";

    if(ui->radioBtnAscend->isChecked() && now != "默认排序")str += "ASC ";
    else if(now != "默认排序")str += "DESC ";

    tabModel->setQuery(str,*__GDB);     //二次查询
}


void courseOverview::on_radioBtnDescend_clicked()
{//切换排序方式
    QString str,now;
    now = ui->comboFields->currentText();
    str = sql + QString("AND score.term = '%1' ").arg(ui->termSelection->currentText());
    if(now == "默认排序");
    else if(now == "学分")str += "ORDER BY credit ";

    if(ui->radioBtnAscend->isChecked() && now != "默认排序")str += "ASC ";
    else if(now != "默认排序")str += "DESC ";

    tabModel->setQuery(str,*__GDB);     //二次查询
}


void courseOverview::on_termSelection_currentTextChanged(const QString &arg1)
{//切换排序方式
    ui->term->setText(ui->termSelection->currentText());    //刷新学期
    QString str,now;
    now = ui->comboFields->currentText();
    str = sql + QString("AND score.term = '%1' ").arg(arg1);
    if(now == "默认排序");
    else if(now == "学分")str += "ORDER BY credit ";

    if(ui->radioBtnAscend->isChecked() && now != "默认排序")str += "ASC ";
    else if(now != "默认排序")str += "DESC ";

    tabModel->setQuery(str,*__GDB);     //二次查询

    //计算学分和
    str = QString("SELECT SUM(course.credit) AS sumCredit FROM score INNER JOIN course ON score.courseID = course.courseID WHERE studentID = %1 AND term = '%2';").arg(user).arg(ui->termSelection->currentText());
    QSqlQuery ser2(str,*__GDB);
    ser2.exec();
    ser2.first();
    int sumCredit= ser2.value("sumCredit").toInt();
    ui->point->setText(QString("%1").arg(sumCredit));

    initClassSchedule();    //初始化课程表
    setClassSchedule(); //加载课程表
}

