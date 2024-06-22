/**
 * @file changepassword.cpp
 * @brief 此文件包含了 `changePassword` 类的定义，用于处理密码更改相关的功能
 */

#include "studentTerminal/headfile/changepassword.h"
#include "ui_changepassword.h"

/**
 * @brief 对输入的字符串进行 MD5 加密
 *
 * @param str 要加密的字符串
 * @return 加密后的 16 进制字符串
 */
QString changePassword::encrypt(const QString &str)
{//字符串MD5算法加密
    QByteArray btArray= str.toLocal8Bit();  //字符串转换为字节数组数据

    QCryptographicHash hash(QCryptographicHash::Md5);  //Md5加密算法
    hash.addData(btArray);  //添加数据到加密哈希值

    QByteArray resultArray =hash.result();  //返回最终的哈希值
    QString md5 =resultArray.toHex();       //转换为16进制字符串
    return  md5;
}

/**
 * @brief `changePassword` 类的构造函数
 *
 * @param parent 父窗口指针
 * @param GDB 数据库指针
 */
changePassword::changePassword(QWidget *parent,QSqlDatabase * GDB)
    : QWidget(parent)
    , __GDB(GDB)
    , ui(new Ui::changePassword)
{
    ui->setupUi(this);

    QSettings set;      //获取注册表
    __user = set.value("Username").toString();
    ui->editUser->setText(__user);      //绑定用户名

}

/**
 * @brief `changePassword` 类的析构函数
 */
changePassword::~changePassword()
{
    delete ui;
}

/**
 * @brief 测试密码的安全性
 *
 * @param pswd 要测试的密码
 * @return 如果密码符合安全要求（8-16 位，包含数字和字母），返回 `true`；否则返回 `false`
 */
bool changePassword::test_password(const QString &pswd)
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

/**
 * @brief 取消按钮的点击响应函数
 */
void changePassword::on_btnCancel_clicked()
{//取消按钮响应
    ui->editNPSWD->setText("");
    ui->editNPSWD2->setText("");
    ui->editPSWD->setText("");
}

/**
 * @brief 确定按钮的点击响应函数
 */
void changePassword::on_btnOK_clicked()
{//"确定"按钮响应
    QString pswd=ui->editPSWD->text().trimmed();    //输入的密码
    QString encrptPSWD=encrypt(pswd);               //对输入的密码进行加密

    QSqlQuery db(*__GDB);                           //搜索数据库
    db.prepare("SELECT password FROM student WHERE studentID = :user");     //搜索密码
    db.bindValue(":user",__user);         //绑定占位符
    db.exec();                          //搜索指令运行

    if(db.first())
    {//搜索到用户名
        __pswd = db.value("password").toString();       //获取密码

        if (encrptPSWD == __pswd)       //如果用户名和密码正确
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


                    QString str = QString("update student set password = '%1' where studentID = %2 ;").arg(npswd).arg(__user);
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



