/*************************************************************************
【文件名】 Student_win2.h
【功能模块和目的】学生上课界面类声明（学生的window2）
【开发者及日期】邱亦文 2020-06-08
【版权信息】
【更改记录】
    2020-06-09 由邱亦文增添“随机语音提问”功能，能够被点名开麦
    2020-06-10 由邱亦文增添“发送注意力”功能实现
    2020-06-17 由邱亦文修改对收到图像的尺寸处理，以及收到屏幕共享停止的消息
    2020-06-17 由邱亦文完善界面
    2020-06-18 由邱亦文增添注释
*************************************************************************/

#ifndef STUDENT_WIN2_H
#define STUDENT_WIN2_H

#include <QDialog>
#include <QCloseEvent>
#include <QTimer>
#include <QTime>
#include "stu_process.h"
#include "mymessagebox.h"
#include "student_ques_win3.h"

namespace Ui {
class Student_win2;
}



/*************************************************************************
【类名】 Student_win2
【功能】学生上课界面类（学生的 window2），显示屏幕共享，是否被提问，上课时长等消息
【接口说明】
    构造函数 Student_win2
    析构函数~Student_win2
    槽函数 on_btnExitClass_clicked 退出课堂
    槽函数 on_StopVoicingBtn_clicked 停止说话

【开发者及日期】邱亦文 2020-06-08
【更改记录】
    2020-06-09 由邱亦文增添“随机语音提问”功能，能够被点名开麦
    2020-06-10 由邱亦文增添“发送注意力”功能实现
    2020-06-17 由邱亦文修改对收到图像的尺寸处理，以及收到屏幕共享停止的消息
    2020-06-17 由邱亦文完善界面
    2020-06-18 由邱亦文增添注释
*************************************************************************/
class Student_win2 : public QDialog
{
    Q_OBJECT

public:
    explicit Student_win2(Stu_Process* const pProc = nullptr, QWidget *parent = nullptr);
    ~Student_win2();
    QString m_sTitleName;

private slots:
    void on_btnExitClass_clicked();
    //收到屏幕共享消息信号
    void RecvScreen(QImage Img);
    //发送注意力消息
    void SendFocus();
    //
    void UpdateTime();

    //被要求打开声音回答问题(在界面上的改变只有按钮和 label，其他都在 process里做完啦)
    //被要求回答问题完毕(在界面上的改变只有按钮和 label，其他都在 process里做完啦)
    void VoiceSending();
    void StopVoiceSending();
    void on_StopVoicingBtn_clicked();

    //接收到一个单选题
    void RecvAQues(QString prob, QString choiceA, QString choiceB, QString choiceC, QString choiceD);

    //2020-06-20 由邱亦文添加 接收到一个多选题
    void RecvMultiQues(QString prob, QString choiceA, QString choiceB, QString choiceC, QString choiceD);

    //2020-06-16 由邱亦文添加，接收到屏幕共享停止
    void RecvScreenStop();

signals:
    void ShouldSendFocusData();

private:
    Ui::Student_win2 *ui;
    Stu_Process* m_pProc2;
    void paintEvent(QPaintEvent *);
    QTimer* m_ptimer;           //定时器 每秒更新时间
    QTime* m_pTimeRecordAll;    //记录总上线时间
    QTime* m_pTimeRecordFocus;  //记录注意力集中的上线时间
    student_ques_win3* m_pStudentWin3;
};

#endif // STUDENT_WIN2_H
