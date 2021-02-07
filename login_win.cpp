/*************************************************************************
【文件名】Login_win.cpp
【功能模块和目的】登录界面类定义
【开发者及日期】邱亦文 2020-05-15
【版权信息】
【更改记录】
    2020-05-22 由邱亦文增添进入教师界面
    2020-05-24 由邱亦文增添进入学生界面
    2020-05-24 由邱亦文增添进入管理员界面
    2020-06-17 由邱亦文完善界面
    2020-06-18 由邱亦文增添注释
    2020-06-21 由邱亦文增添三次登录失败，退出系统
*************************************************************************/
#include "login_win.h"
#include "ui_login_win.h"
#include "teacher_win.h"
#include "student_win.h"
#include "admin_win.h"
#include "user.h"
#include "mymessagebox.h"

Login_win::Login_win(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login_win)
{
    ui->setupUi(this);
    this->resize(800,600);
    this->setWindowTitle("Welcome To ThunderClass");
    this->setWindowIcon(QIcon("material/thunder.png"));
    //this->setWindowFlag(Qt::FramelessWindowHint);
    ui->EnterBtn->setShortcut(Qt::Key_Enter);
    ui->EnterBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->ExitBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->PasswdLine->setEchoMode(QLineEdit::Password);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}

Login_win::~Login_win()
{
    delete ui;
}


void Login_win::on_EnterBtn_clicked()
{
    QMessageBox msgBox;
    Teacher_win* pTeacherWindow;
    Student_win* pStudentWindow;
    Admin_Win* pAdminWindow;
    Result res = Proc.Login(ui->UserLine->text(), ui->PasswdLine->text());
    switch (res) {
    case LOG_FAIL :
        MyMessageBox::MessageOnlyOk_Error("三次了！哦豁完蛋程序崩溃……");
        this->close();
        break;
    case NONE :
        MyMessageBox::MessageOnlyOk_Error("该用户没有权限");
        break;
    case A :
        this->setWindowOpacity(0.0);
        pAdminWindow = new Admin_Win(this);
        pAdminWindow->exec();
        this->setWindowOpacity(1.0);
        delete pAdminWindow;
        pAdminWindow = nullptr;
        break;
    case T :
        this->setWindowOpacity(0.0);
        pTeacherWindow = new Teacher_win(this);
        pTeacherWindow->setWindowTitle("Thunderclass - Welcome Teacher " + QString(User::GetLoginedUser()->GetName().c_str()) + "!");
        pTeacherWindow->exec();
        this->setWindowOpacity(1.0);
        delete pTeacherWindow;
        pTeacherWindow = nullptr;
        break;
    default:
        this->setWindowOpacity(0.0);
        pStudentWindow = new Student_win(this);
        pStudentWindow->setWindowTitle("Thunderclass - Welcome Student " + QString(User::GetLoginedUser()->GetName().c_str()) + "!");
        pStudentWindow->exec();
        this->setWindowOpacity(1.0);
        delete pStudentWindow;
        pStudentWindow = nullptr;
    }
}
void Login_win::on_ExitBtn_clicked()
{
    this->close();
}
