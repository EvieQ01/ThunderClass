/*************************************************************************
【文件名】 Teacher_quesResult_win5.h
【功能模块和目的】教师提问收集答案结果界面类声明（教师的window5）
【开发者及日期】邱亦文 2020-06-15
【版权信息】
【更改记录】
    2020-06-20 由邱亦文增添注释
*************************************************************************/
#ifndef TEACHER_QUESRESULT_WIN5_H
#define TEACHER_QUESRESULT_WIN5_H

#include <QDialog>
#include <QTimer>
#include "tchr_process.h"

namespace Ui {
class Teacher_quesResult_win5;
}


class Teacher_quesResult_win5 : public QDialog
{
    Q_OBJECT

public:
    explicit Teacher_quesResult_win5(Teacher_Process* pProc, QWidget *parent = nullptr);
    ~Teacher_quesResult_win5();
    void closeEvent (QCloseEvent* e) override;

private slots:
    void on_ExitBtn_clicked();
    void Updateinfo();

private:
    Ui::Teacher_quesResult_win5 *ui;
    Teacher_Process* m_pProc5;
};

#endif // TEACHER_QUESRESULT_WIN5_H
