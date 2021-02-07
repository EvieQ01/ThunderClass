/*************************************************************************
【文件名】 Teacher_win.h
【功能模块和目的】教师登入界面类声明（教师的window1）
【开发者及日期】邱亦文 2020-05-23
【版权信息】
【更改记录】
    2020-06-17 由邱亦文完善界面
    2020-06-18 由邱亦文增添异常处理
    2020-06-18 由邱亦文增添注释
*************************************************************************/
#ifndef TEACHER_WIN_H
#define TEACHER_WIN_H

#include <QDialog>
#include "teacher_win2.h"
#include "tchr_process.h"

namespace Ui {
class Teacher_win;
}

/*************************************************************************
【类名】 Teacher_win
【功能】教师登入界面类（教师window1）
【接口说明】
    构造函数 Teacher_win
    析构函数~Teacher_win
    槽函数 on_beginBtn_clicked 尝试开始上课，建立一个server
    槽函数 on_ExitBtn_clicked 退出登录

【开发者及日期】邱亦文 2020-05-23
【更改记录】
    2020-06-17 由邱亦文完善界面
    2020-06-18 由邱亦文增添异常处理
    2020-06-18 由邱亦文增添注释
*************************************************************************/
class Teacher_win : public QDialog
{
    Q_OBJECT

public:
    explicit Teacher_win(QWidget *parent = nullptr);
    ~Teacher_win();

private slots:
    //进入课堂
    void on_beginBtn_clicked();
    //退出登录
    void on_exitBtn_clicked();


private:
    Ui::Teacher_win *ui;
    Teacher_win2* m_pTeacherwin2;
    Teacher_Process* m_pProc;
    void paintEvent(QPaintEvent *);
};

#endif // TEACHER_WIN_H
