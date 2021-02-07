/*************************************************************************
【文件名】 Student_win.h
【功能模块和目的】学生登入界面类声明（学生的window1）
【开发者及日期】邱亦文 2020-06-08
【版权信息】
【更改记录】
    2020-06-18 由邱亦文增添异常处理
    2020-06-18 由邱亦文增添注释
*************************************************************************/

#ifndef STUDENT_WIN_H
#define STUDENT_WIN_H

#include <QDialog>
#include "stu_process.h"
#include "student_win2.h"

namespace Ui {
class Student_win;
}


/*************************************************************************
【类名】 Student_win
【功能】学生登入界面类（学生window1）
【接口说明】
    构造函数 Student_win
    析构函数~Student_win
    槽函数 on_btnEnterClass_clicked 进入课堂
    槽函数 on_ExitBtn_clicked 退出登录

【开发者及日期】邱亦文 2020-06-08
【更改记录】
    2020-06-18 由邱亦文增添异常处理
    2020-06-18 由邱亦文增添注释
*************************************************************************/

class Student_win : public QDialog
{
    Q_OBJECT

public:
    explicit Student_win(QWidget *parent = nullptr);
    ~Student_win();

private slots:
    void on_btnEnterClass_clicked();
    void on_ExitBtn_clicked();

private:
    Ui::Student_win *ui;
    Stu_Process* m_pProc;
    Student_win2* m_pStudentWin2;
    void paintEvent(QPaintEvent *);
};

#endif // STUDENT_WIN_H
