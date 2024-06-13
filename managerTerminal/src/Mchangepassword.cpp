#include "managerTerminal/headfile/Mchangepassword.h"
#include "ui_Mchangepassword.h"

QString MchangePassword::encrypt(const QString &str)
{//字符串MD5算法加密
    QByteArray btArray= str.toLocal8Bit();  //字符串转换为字节数组数据

    QCryptographicHash hash(QCryptographicHash::Md5);  //Md5加密算法
    hash.addData(btArray);  //添加数据到加密哈希值

    QByteArray resultArray =hash.result();  //返回最终的哈希值
    QString md5 =resultArray.toHex();       //转换为16进制字符串
    return  md5;
}

MchangePassword::MchangePassword(QWidget *parent,QSqlDatabase * GDB)
    : QWidget(parent)
    , __GDB(GDB)
    , ui(new Ui::MchangePassword)
{
    ui->setupUi(this);
}

MchangePassword::~MchangePassword()
{
    delete ui;
}

bool MchangePassword::test_password(const QString &pswd)
{//测试密码安全性
    if(pswd.size() >= 8)
        if(pswd.size() <= 16){
            bool num=false,letter=false;
            for(auto ip = pswd.begin();ip != pswd.end();ip++)
            {//测试密码的可行性
                        if(ip->isNumber())num=true;
                        else if(ip->isLetter())letter=true;
                        else return false;
            }
            if(num == true && letter == true)return true;
            else return false;
        }
        else return false;
    else return false;
}

void MchangePassword::on_btnCancel_clicked()
{//取消按钮响应
    ui->editNPSWD->setText("");
    ui->editNPSWD2->setText("");
    ui->editPSWD->setText("");
}

void MchangePassword::on_btnOK_clicked()
{//"确定"按钮响应
    QString pswd=ui->editPSWD->text().trimmed();    //输入的密码
    QString encrptPSWD=encrypt(pswd);               //对输入的密码进行加密

    QSqlQuery db(*__GDB);                           //搜索数据库
    db.prepare("SELECT settingContent FROM setting WHERE settingName = 'managerPassword' ");     //搜索密码
    db.exec();                          //搜索指令运行

    if(db.first())
    {//搜索
        __pswd = db.value("settingContent").toString();       //获取密码

        if (encrptPSWD == __pswd)       //如果密码正确
        {
            QString npswd = ui->editNPSWD->text().trimmed();    //读入新密码
            if(npswd != ui->editNPSWD2->text().trimmed())
            {   //两次输入不一样
               QMessageBox::critical(this, "错误", "两次新密码输入不一样");
                on_btnCancel_clicked();     //取消
               return;
            }
            else
            {
                if(test_password(npswd))
                {//修改操作
                    npswd = encrypt(npswd);     //加密


                    QString str = QString("update setting set settingContent = '%1' where settingName = 'managerPassword' ;").arg(npswd);
                    QSqlQuery sql(str,*__GDB);
                    sql.exec();

                    QMessageBox::information(this, "修改密码", "修改密码成功！");
                    on_btnCancel_clicked();     //取消
                    return;
                }
                else
                {
                    QMessageBox::critical(this, "错误", "密码太简单，密码应该是8-16位由数字和字母共同组成的不包含特殊字符的字符串。");
                    on_btnCancel_clicked();     //取消
                    return;
                }
            }
            return;
        }
        else
        {
            QMessageBox::warning(this, "错误提示", "密码错误");
            on_btnCancel_clicked();     //取消
            return;
        }
    }
    else
        return;
}



