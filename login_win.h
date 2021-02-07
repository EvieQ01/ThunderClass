/*************************************************************************
【文件名】 Login_win.h
【功能模块和目的】登录界面类声明
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
#ifndef LOGIN_WIN_H
#define LOGIN_WIN_H

#include "login_process.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Login_win; }
QT_END_NAMESPACE

/*************************************************************************
【类名】Admin_Win
【功能】管理员界面类
【接口说明】
    构造函数 Admin_Win
    析构函数~Admin_Win
    槽函数 on_EnterBtn_clicked 登录
    槽函数 on_ExitBtn_clicked 退出程序

【开发者及日期】邱亦文 2020-05-15
【更改记录】
    2020-05-22 由邱亦文增添进入教师界面
    2020-05-24 由邱亦文增添进入学生界面
    2020-05-24 由邱亦文增添进入管理员界面
    2020-06-17 由邱亦文完善界面
    2020-06-18 由邱亦文增添注释
    2020-06-21 由邱亦文增添三次登录失败，退出系统
*************************************************************************/
class Login_win : public QDialog
{
    Q_OBJECT

public:
    Login_win(QWidget *parent = nullptr);
    ~Login_win();

private slots:
    void on_EnterBtn_clicked();

    void on_ExitBtn_clicked();

private:
    Ui::Login_win *ui;
    LoginProcess Proc;
};
#endif // LOGIN_WIN_H
