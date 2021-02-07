/*************************************************************************
【文件名】 Teacher_exit_win3.h
【功能模块和目的】教师下课后界面类声明（教师的window3）
【开发者及日期】邱亦文 2020-06-10
【版权信息】
【更改记录】
    2020-06-17 由邱亦文完善界面
    2020-06-18 由邱亦文增添注释
*************************************************************************/
#ifndef TEACHER_EXIT_WIN3_H
#define TEACHER_EXIT_WIN3_H

#include <QDialog>
#include <QTimer>
#include "tchr_process.h"

namespace Ui {
class Teacher_exit_win3;
}

/*************************************************************************
【类名】 Teacher_exit_win3
【功能】教师下课后界面类（教师的window3），显示全部学生的注意力情况
【接口说明】
    构造函数 Teacher_exit_win3
    析构函数 ~Teacher_exit_win3

【开发者及日期】邱亦文 2020-06-10
【版权信息】
【更改记录】
    2020-06-17 由邱亦文完善界面
    2020-06-18 由邱亦文增添注释
*************************************************************************/
class Teacher_exit_win3 : public QDialog
{
    Q_OBJECT

public:
    explicit Teacher_exit_win3(Teacher_Process* const pProc = nullptr, QWidget *parent = nullptr);
    ~Teacher_exit_win3();

private slots:
    void on_pushButton_clicked();

    void TimerEvent();
    void MustUpdate();

private:
    Ui::Teacher_exit_win3 *ui;
    Teacher_Process* m_pProc3;
    QTimer* m_pTimer;
    void ShowFocusDataPiece() const;
    void SaveFocusToFile(ofstream& OutFile) const;
};

#endif // TEACHER_EXIT_WIN3_H
