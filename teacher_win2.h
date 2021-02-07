/*************************************************************************
【文件名】 Teacher_win2.h
【功能模块和目的】教师上课中界面类声明（教师的window2）
【开发者及日期】邱亦文 2020-06-06
【版权信息】
【更改记录】
    2020-06-09 由邱亦文增添“随机语音提问”功能，能够进行随机开麦
    2020-06-09 由邱亦文增添“收集注意力”功能
    2020-06-12 由邱亦文完善“随机语音提问”功能，能够进行语音向其他人转播，并由教师端/同学端结束语音提问
    2020-06-15 由邱亦文增添“发题”功能，能够发送文本题目
    2020-06-16 由邱亦文完善“发题”功能，能够收到学生答案，并由教师端一键收题
    2020-06-17 由邱亦文完善界面
    2020-06-18 由邱亦文增添异常处理
    2020-06-18 由邱亦文增添注释
*************************************************************************/
#ifndef TEACHER_WIN2_H
#define TEACHER_WIN2_H

#include <QDialog>
#include <QTimer>
#include <QMessageBox>
#include "tchr_process.h"
#include "teacher_exit_win3.h"
#include "teacher_ques_win4.h"
#include "user.h"
#include "mymessagebox.h"

namespace Ui {
class Teacher_win2;
}

/*************************************************************************
【类名】 Teacher_win2
【功能】教师上课中界面（教师的window2）
      可以进行屏幕共享，语音共享，随机点名，发送题目4大功能
【接口说明】
    构造函数 Teacher_win2
    析构函数~Teacher_win2
    槽函数 on_ScreenShareBtn_clicked 共享屏幕
    槽函数 on_VoiceShareBtn_clicked 共享语音
    槽函数 on_DiapatchQuesBtn_clicked 发送题目
    槽函数 on_RandomCallBtn_clicked 随即语音提问
    槽函数 on_btnEndClass_clicked 推出课堂并收集注意力消息
    槽函数 on_StopScreenBtn_clicked();
    槽函数 on_StopVoiceBtn_clicked();
    槽函数 on_StopRandomCallBtn_clicked();几个停止功能

【开发者及日期】邱亦文 2020-06-08
【更改记录】
    2020-06-09 由邱亦文增添“随机语音提问”功能，能够进行随机开麦
    2020-06-09 由邱亦文增添“收集注意力”功能
    2020-06-12 由邱亦文完善“随机语音提问”功能，能够进行语音向其他人转播，并由教师端/同学端结束语音提问
    2020-06-15 由邱亦文增添“发题”功能，能够发送文本题目
    2020-06-16 由邱亦文完善“发题”功能，能够收到学生答案，并由教师端一键收题
    2020-06-17 由邱亦文完善界面
    2020-06-18 由邱亦文增添异常处理
    2020-06-18 由邱亦文增添注释
*************************************************************************/

class Teacher_win2 : public QDialog
{
    Q_OBJECT

public:
    //被teacher的第一个窗口创建，pProc跟第一个窗口一样
    explicit Teacher_win2(Teacher_Process* const pProc2 = nullptr, QWidget *parent = nullptr);
    ~Teacher_win2();

private slots:

    void RecvStudentLogin(QString Name);

    void RecvStudentLogout(QString Name);

    void CallingToStu(QString Name);

    void UpdateControlEnabled();

   // void RecvChat(QString Msg);

    void on_ScreenShareBtn_clicked();

    void on_VoiceShareBtn_clicked();

    void on_btnEndClass_clicked();

    void on_StopScreenBtn_clicked();

    void on_StopVoiceBtn_clicked();

    void on_RandomCallBtn_clicked();

    void RandomCallEnd();

    void on_StopRandomCallBtn_clicked();

    void on_DiapatchQuesBtn_clicked();

private:
    Ui::Teacher_win2 *ui;
    Teacher_Process* m_pProc2;
    Teacher_exit_win3* m_pTeacherWin3;
    Teacher_ques_win4* m_pTeacherWin4;
    QTimer* m_pTimer;
    void paintEvent(QPaintEvent *);

};

#endif // TEACHER_WIN2_H
