/*************************************************************************
【文件名】 teacher_ques_win4.cpp
【功能模块和目的】教师提问界面类声明（教师的window4）
【开发者及日期】邱亦文 2020-06-15
【版权信息】
【更改记录】
    2020-06-20 由邱亦文增添注释
*************************************************************************/
#ifndef TEACHER_QUES_WIN4_H
#define TEACHER_QUES_WIN4_H

#include <QDialog>
#include <QTimer>
#include "tchr_process.h"
#include "mymessagebox.h"
#include "teacher_quesresult_win5.h"
namespace Ui {
class Teacher_ques_win4;
}

class Teacher_ques_win4 : public QDialog
{
    Q_OBJECT

public:
    explicit Teacher_ques_win4(Teacher_Process* const aProc, QWidget *parent = nullptr);
    ~Teacher_ques_win4();
    void closeEvent (QCloseEvent* e) override;

private slots:
    void UpdateControlEnabled();
    void on_SendQuesBtn_clicked();

    void on_CollectAnswerBtn_clicked();

    void on_ShowInfoBtn_clicked();

private:
    Ui::Teacher_ques_win4 *ui;
    Teacher_Process* m_pProc4;
    QTimer* m_pTimer;
    Teacher_quesResult_win5* m_pTeacherWin5;
    void paintEvent(QPaintEvent *) override;
};

#endif // TEACHER_QUES_WIN4_H
