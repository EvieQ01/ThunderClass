/*************************************************************************
【文件名】 teacher_quesresult_win5.cpp
【功能模块和目的】教师提问收集答案结果界面类定义（教师的window5）
【开发者及日期】邱亦文 2020-06-15
【版权信息】
【更改记录】
    2020-06-20 由邱亦文增添注释
*************************************************************************/
#include "teacher_quesresult_win5.h"
#include "ui_teacher_quesresult_win5.h"
#include <QCloseEvent>

Teacher_quesResult_win5::Teacher_quesResult_win5(Teacher_Process* pProc,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Teacher_quesResult_win5), m_pProc5(pProc)
{
    ui->setupUi(this);
    for (int i = 0;i < 5;i++) {
        ui->staticsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString("0")));
    }
    connect(m_pProc5, SIGNAL(RecvAnswerResult()), this, SLOT(Updateinfo()));

}

Teacher_quesResult_win5::~Teacher_quesResult_win5()
{
    delete ui;
}

void Teacher_quesResult_win5::on_ExitBtn_clicked()
{
    this->hide();
}

void Teacher_quesResult_win5::Updateinfo() {
    ui->InfmationTable->insertRow(0);
    ui->InfmationTable->setItem(0, 0, new QTableWidgetItem(QString::fromStdString(m_pProc5->GetLastAnswer_name())));
    ui->InfmationTable->setItem(0, 1, new QTableWidgetItem(QString::fromStdString(m_pProc5->GetLastAnswer_choice())));
    ui->InfmationTable->setItem(0, 2, new QTableWidgetItem(QString::fromStdString(m_pProc5->GetLastAnswer_time())));
    for (int i = 0;i < 5;i++) {
        ui->staticsTable->item(i, 0)->setText(QString::fromStdString(to_string(m_pProc5->GetAnswerCount(i + 1))));
    }
    ui->staticsTable->item(4, 0)->setText(QString::fromStdString(to_string(m_pProc5->GetAnswerCount(0))));
    ui->InfmationTable->resizeColumnsToContents();
    ui->staticsTable->resizeColumnsToContents();
}

void Teacher_quesResult_win5::closeEvent (QCloseEvent* e) {
    //不要关闭，一直默默地更新
    if (m_pProc5->IsInClass) {
        e->ignore();
        this->hide();
    }
}
