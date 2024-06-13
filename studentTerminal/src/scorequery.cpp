#include "studentTerminal/headfile/scorequery.h"
#include "ui_scorequery.h"

scoreQuery::scoreQuery(QWidget *parent,QSqlDatabase * GDB)
    : QWidget(parent)
    , __GDB(GDB)
    , ui(new Ui::scoreQuery)
{
    ui->setupUi(this);

    QSettings set;
    QString user = set.value("Username").toString();
    //创建数据模型，打开数据表
    tabModel=new QSqlQueryModel(this);	//数据模型

    //SQL搜索语句
    sql = QString("SELECT processEvaluation, finalExam, gradePoint, term, course.name AS courseName, course.type AS courseType, course.credit AS credit, teacher.name AS teacherName "
                  "FROM score "
                  "INNER JOIN course ON score.courseID = course.courseID "
                  "INNER JOIN teacher ON course.teacherID = teacher.teacherID "
                  "WHERE score.studentID = %1 AND processEvaluation IS NOT NULL ").arg(user);

    tabModel->setQuery(sql,*__GDB);			//设置数据表

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
    tabModel->setHeaderData(rec.indexOf("processEvaluation"), Qt::Horizontal, "过程评价");
    tabModel->setHeaderData(rec.indexOf("finalExam"), Qt::Horizontal, "期末成绩");
    tabModel->setHeaderData(rec.indexOf("gradePoint"), Qt::Horizontal, "绩点");
    tabModel->setHeaderData(rec.indexOf("term"), Qt::Horizontal, "学期");


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
    dataMapper->addMapping(ui->processEvaluation,rec.indexOf("processEvaluation"));
    dataMapper->addMapping(ui->gradePoint,rec.indexOf("gradePoint"));
    dataMapper->addMapping(ui->finalExam,rec.indexOf("finalExam"));
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

scoreQuery::~scoreQuery()
{
    delete ui;
}

void scoreQuery::on_comboFields_currentTextChanged(const QString &arg1)
{//切换排序方式
    Q_UNUSED(arg1)
    QString str,now;
    now = ui->comboFields->currentText();
    if(now == "默认排序")
    {
        ui->radioBtnAscend->setChecked(true);
        ui->radioBtnDescend->setChecked(false);
        ui->radioBtnAscend->setEnabled(false);
        ui->radioBtnDescend->setEnabled(false);
        str = sql;
    }
    else
    {
        ui->radioBtnAscend->setEnabled(true);
        ui->radioBtnDescend->setEnabled(true);
        if(now == "学分")str = sql + "ORDER BY credit ";
        else if(now == "绩点")str = sql + "ORDER BY gradePoint ";
        else if(now == "过程评价")str = sql + "ORDER BY processEvaluation ";
        else if(now == "期末成绩")str = sql + "ORDER BY finalExam ";
    }

    if(ui->radioBtnAscend->isChecked() && now != "默认排序")str += "ASC ";
    else if(now != "默认排序")str += "DESC ";

    tabModel->setQuery(str,*__GDB);
}


void scoreQuery::on_radioBtnAscend_clicked()
{//切换排序方式
    QString str,now;
    now = ui->comboFields->currentText();
    if(now == "默认排序") str = sql;
    else if(now == "学分")str = sql + "ORDER BY credit ";
    else if(now == "绩点")str = sql + "ORDER BY gradePoint ";
    else if(now == "过程评价")str = sql + "ORDER BY processEvaluation ";
    else if(now == "期末成绩")str = sql + "ORDER BY finalExam ";

    if(ui->radioBtnAscend->isChecked() && now != "默认排序")str += "ASC ";
    else if(now != "默认排序")str += "DESC ";

    tabModel->setQuery(str,*__GDB);
}


void scoreQuery::on_radioBtnDescend_clicked()
{//切换排序方式
    QString str,now;
    now = ui->comboFields->currentText();
    if(now == "默认排序") str = sql;
    else if(now == "学分")str = sql + "ORDER BY credit ";
    else if(now == "绩点")str = sql + "ORDER BY gradePoint ";
    else if(now == "过程评价")str = sql + "ORDER BY processEvaluation ";
    else if(now == "期末成绩")str = sql + "ORDER BY finalExam ";

    if(ui->radioBtnAscend->isChecked() && now != "默认排序")str += "ASC ";
    else if(now != "默认排序")str += "DESC ";

    tabModel->setQuery(str,*__GDB);
}

