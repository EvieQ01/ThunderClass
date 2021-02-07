/*************************************************************************
【文件名】 student_ques_win3.h
【功能模块和目的】学生答题界面类声明（学生的window3）
【开发者及日期】邱亦文 2020-06-16
【版权信息】
【更改记录】
    2020-06-17 由邱亦文完善界面
    2020-06-18 由邱亦文增添注释
*************************************************************************/
#ifndef student_ques_win3_H
#define student_ques_win3_H

#include <QDialog>
#include "stu_process.h"

namespace Ui {
class student_ques_win3;
}


/*************************************************************************
【类名】 student_ques_win3
【功能】学生答题界面类（学生的 window3），在最上方显示屏幕要求作答
【接口说明】
    构造函数 student_ques_win3
    析构函数~student_ques_win3
    槽函数 on_ConfirmAnswBtn_clicked 确认作答

【开发者及日期】邱亦文 2020-06-16
【更改记录】
    2020-06-17 由邱亦文完善界面
    2020-06-18 由邱亦文增添注释
*************************************************************************/
class student_ques_win3 : public QDialog
{
    Q_OBJECT

public:
    explicit student_ques_win3(Stu_Process* pProc, QWidget *parent = nullptr);
    ~student_ques_win3();
    void SetQuestion(QString prob, QString choiceA, QString choiceB, QString choiceC, QString choiceD);
    void SetType(Questiontype type);
private slots:
    void on_ConfirmAnswBtn_clicked();
    void TimerEvent();
    void RecvFinishChoosing();
private:
    Ui::student_ques_win3 *ui;
    Stu_Process* m_pProc3;
    QTime* m_pTime;
    QTimer* m_pTimer;
    void paintEvent(QPaintEvent *);
    Questiontype m_type = Q_UNKNOWN;
};

#endif // student_ques_win3_H
