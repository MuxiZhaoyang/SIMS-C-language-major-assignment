#include "studentTerminal/headfile/courseinformation.h"
#include "ui_courseinformation.h"

courseInformation::courseInformation(QWidget *parent,QSqlDatabase * GDB)
    : QWidget(parent)
    , __GDB(GDB)
    , ui(new Ui::courseInformation)
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

    //创建界面组件与数据模型的字段之间的数据映射
    dataMapper= new QDataWidgetMapper(this);
    dataMapper->setModel(tabModel);		//设置数据模型
    //界面组件与数据模型的具体字段之间的联系
    dataMapper->addMapping(ui->term,rec.indexOf("term"));
    dataMapper->addMapping(ui->courseName,rec.indexOf("courseName"));
    dataMapper->addMapping(ui->courseType,rec.indexOf("courseType"));
    dataMapper->addMapping(ui->teacherName,rec.indexOf("teacherName"));
    dataMapper->addMapping(ui->point,rec.indexOf("credit"));
    dataMapper->addMapping(ui->schoolName,rec.indexOf("schoolName"));
    dataMapper->addMapping(ui->place,rec.indexOf("place"));
    dataMapper->addMapping(ui->classTime,rec.indexOf("classTime"));
    dataMapper->toFirst();  //移动到首记录

    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,       //切换映射
            this, [this](const QModelIndex &current, const QModelIndex &previous) {         //Lamda函数
                Q_UNUSED(previous)
                if (current.isValid()) {
                    dataMapper->setCurrentModelIndex(current);
                }
            });

    ui->radioBtnAscend->setChecked(true);
    ui->radioBtnDescend->setChecked(false);
    ui->radioBtnAscend->setEnabled(false);
    ui->radioBtnDescend->setEnabled(false);
}

courseInformation::~courseInformation()
{
    delete ui;
}

void courseInformation::on_comboFields_currentTextChanged(const QString &arg1)
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


    if(tabModel->columnCount() > 0)dataMapper->toFirst();   //刷新当前选框
    else
    {
        ui->courseName->setText("");
        ui->courseType->setText("");
        ui->teacherName->setText("");
        ui->point->setText("");
        ui->schoolName->setText("");
        ui->place->setText("");
        ui->classTime->setText("");
    }
}


void courseInformation::on_radioBtnAscend_clicked()
{//切换排序方式
    QString str,now;
    now = ui->comboFields->currentText();
    str = sql + QString("AND score.term = '%1' ").arg(ui->termSelection->currentText());
    if(now == "默认排序");
    else if(now == "学分")str += "ORDER BY credit ";

    if(ui->radioBtnAscend->isChecked() && now != "默认排序")str += "ASC ";
    else if(now != "默认排序")str += "DESC ";

    tabModel->setQuery(str,*__GDB);     //二次查询

    if(tabModel->columnCount() > 0)dataMapper->toFirst();   //刷新当前选框
    else
    {
        ui->courseName->setText("");
        ui->courseType->setText("");
        ui->teacherName->setText("");
        ui->point->setText("");
        ui->schoolName->setText("");
        ui->place->setText("");
        ui->classTime->setText("");
    }
}


void courseInformation::on_radioBtnDescend_clicked()
{//切换排序方式
    QString str,now;
    now = ui->comboFields->currentText();
    str = sql + QString("AND score.term = '%1' ").arg(ui->termSelection->currentText());
    if(now == "默认排序");
    else if(now == "学分")str += "ORDER BY credit ";

    if(ui->radioBtnAscend->isChecked() && now != "默认排序")str += "ASC ";
    else if(now != "默认排序")str += "DESC ";

    tabModel->setQuery(str,*__GDB);     //二次查询

    if(tabModel->columnCount() > 0)dataMapper->toFirst();   //刷新当前选框
    else
    {
        ui->courseName->setText("");
        ui->courseType->setText("");
        ui->teacherName->setText("");
        ui->point->setText("");
        ui->schoolName->setText("");
        ui->place->setText("");
        ui->classTime->setText("");
    }
}


void courseInformation::on_termSelection_currentTextChanged(const QString &arg1)
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

    QSqlQuery ser(str,*__GDB);
    ser.exec();
    if(ser.first())
    {//刷新当前选框
        dataMapper->toFirst();
        ui->comboFields->setEnabled(true);
    }
    else
    {
        ui->courseName->setText("");
        ui->courseType->setText("");
        ui->teacherName->setText("");
        ui->point->setText("");
        ui->schoolName->setText("");
        ui->place->setText("");
        ui->classTime->setText("");
    }
}

