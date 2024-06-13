#include "login.h"
#include "ui_login.h"

//读取存储的用户名和密码, 密码是经过加密的
void Login::readSettings()
{
    QSettings settings;     //创建QSettings对象
    bool saved=settings.value("saved",false).toBool();      //读取saved键的值

    if (saved)
    {//是否保存过注册表
        ui->editUser->setText(__user);
        __user=settings.value("Username").toString();   //读取 Username 键的值
    }
    ui->editUser->setText(__user);          //设置注册表用户名
    ui->chkBoxSave->setChecked(saved);      //设置注册表saved状态
}

//保存用户名，密码等设置
void Login::writeSettings()
{
    QSettings   settings;   //注册表键组
    settings.setValue("Username",__user);       //用户名
    settings.setValue("saved",ui->chkBoxSave->isChecked());
}

QString Login::encrypt(const QString &str)
{ //字符串MD5算法加密
    QByteArray btArray= str.toLocal8Bit();  //字符串转换为字节数组数据

    QCryptographicHash hash(QCryptographicHash::Md5);  //Md5加密算法
    hash.addData(btArray);  //添加数据到加密哈希值

    QByteArray resultArray =hash.result();  //返回最终的哈希值
    QString md5 =resultArray.toHex();       //转换为16进制字符串
    return  md5;
}

void Login::mousePressEvent(QMouseEvent *event)
{ //鼠标按键被按下
    if (event->button() == Qt::LeftButton)
    {
        __moving = true;
        __lastPos = event->globalPosition().toPoint() - this->pos();
    }
    return QDialog::mousePressEvent(event);
}

void Login::mouseMoveEvent(QMouseEvent *event)
{ //鼠标按下左键移动
    QPoint eventPos=event->globalPosition().toPoint();

    if (__moving && (event->buttons() & Qt::LeftButton)
        && (eventPos-__lastPos).manhattanLength() > QApplication::startDragDistance())
    {
        move(eventPos-__lastPos);
        __lastPos = eventPos - this->pos();
    }
    return QDialog::mouseMoveEvent(event);
}

void Login::mouseReleaseEvent(QMouseEvent *event)
{ //鼠标按键被释放
    __moving=false;     //停止移动
    event->accept();
}

Login::Login(QSqlDatabase * GDB,QWidget *parent) :
    QDialog(parent),
    __GDB(GDB),
    ui(new Ui::Login)

{
    ui->setupUi(this);

    ui->editPSWD->setEchoMode(QLineEdit::Password);     //设置为密码输入模式
    //this->setAttribute(Qt::WA_DeleteOnClose);           //对话框关闭时自动删除
    this->setWindowFlags(Qt::SplashScreen);             //设置为SplashScreen, 窗口无边框,不在任务栏显示

    QApplication::setOrganizationName("SMS");                       //注册表组织
    QApplication::setApplicationName("studentManagementSystem");    //应用程序名
    readSettings(); //读取存储的用户名和密码
}

Login::~Login()
{
    delete ui;
}

void Login::on_btnOK_clicked()
{//"确定"按钮响应

    QString user=ui->editUser->text().trimmed();    //输入的用户名
    QString pswd=ui->editPSWD->text().trimmed();    //输入的密码
    QString encrptPSWD=encrypt(pswd);               //对输入的密码进行加密
    QSqlQuery db(*__GDB);                           //搜索数据库

    if(ui->comboBox->currentText() == "学生端")
    {
        db.prepare("SELECT password FROM student WHERE studentID = :user");     //搜索密码
        db.bindValue(":user",user);         //绑定占位符
        db.exec();                          //搜索指令运行
        if(db.first())
        {//搜索到用户名
            __pswd = db.value("password").toString();       //获取密码

            //qDebug()<<__pswd;

            if (encrptPSWD==__pswd)       //如果用户名和密码正确
            {
                __user = user;          //设置用户名
                writeSettings();
                this->accept();     //accept()，关闭对话框
            }
            else
            {
                __tryCount++;       //错误次数
                if (__tryCount>3)
                {
                    QMessageBox::critical(this, "错误", "输入错误次数太多，强行退出");
                    this->reject();
                }
                else
                    QMessageBox::warning(this, "错误提示", "密码错误");
            }
        }
        else
            QMessageBox::warning(this, "错误提示", "用户名不存在");
    }
    else
    {
        db.prepare("SELECT settingContent FROM setting WHERE settingName = 'manager' ");     //搜索管理员名
        db.exec();                          //搜索指令运行
        db.first();
        QString man = db.value("settingContent").toString();


        db.prepare("SELECT settingContent FROM setting WHERE settingName = 'managerPassword' ");     //搜索密码
        db.exec();                          //搜索指令运行
        db.first();
        QString manPSWD = db.value("settingContent").toString();

        //qDebug()<<manPSWD;

        if(user == man && encrptPSWD == manPSWD)
        {
            loginUser = "Manager";
            this->accept();     //accept()，关闭对话框
        }
        else
        {
            __tryCount++;       //错误次数
            if (__tryCount>3)
            {
                QMessageBox::critical(this, "错误", "输入错误次数太多，强行退出");
                this->reject();
            }
            else
                QMessageBox::warning(this, "错误提示", "用户名或密码错误");        }
    }
}
