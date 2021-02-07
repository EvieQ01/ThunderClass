/*************************************************************************
【文件名】 Student_win.cpp
【功能模块和目的】学生登入界面类定义（学生window1）
【开发者及日期】邱亦文 2020-06-08
【版权信息】
【更改记录】
    2020-06-18 由邱亦文增添异常处理
    2020-06-18 由邱亦文增添注释
*************************************************************************/

#include "student_win.h"
#include "ui_student_win.h"
#include "mymessagebox.h"
#include <QPainter>

Student_win::Student_win(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Student_win)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    ui->btnEnterClass->setShortcut(Qt::Key_Enter);
    m_pProc = new Stu_Process(this);

}

Student_win::~Student_win()
{
    delete m_pProc;
    m_pProc = nullptr;
    delete ui;
}

void Student_win::paintEvent(QPaintEvent *) {
    QPixmap pixmap = QPixmap(":/material/StuBack0.png").scaled(this->size());
    QPainter painter(this);
    painter.drawPixmap(this->rect(), pixmap);
    ui->ExitBtn->setIcon(QIcon("material/Stop.png"));
    ui->btnEnterClass->setCursor(QCursor(Qt::PointingHandCursor));
    ui->ExitBtn->setCursor(QCursor(Qt::PointingHandCursor));

}

void Student_win::on_btnEnterClass_clicked()
{
    try {
        if (m_pProc->EnterClass(ui->edtIP->text())) {
            this->setWindowOpacity(0.0);
            m_pStudentWin2 = new Student_win2(m_pProc, this);
            m_pStudentWin2->setWindowFlags(Qt::Dialog | Qt::WindowMinMaxButtonsHint);
            m_pStudentWin2->setWindowTitle(this->windowTitle());
            m_pStudentWin2->m_sTitleName = this->windowTitle();
            m_pStudentWin2->exec();
            this->setWindowOpacity(1.0);
            delete m_pStudentWin2;
            m_pStudentWin2 = nullptr;
        }
        else {
            MyMessageBox::MessageOnlyOk_Information("Failed to enter a class! Please check your port number and network connection!");
        }


    } catch (runtime_error& e) {
        MyMessageBox::MessageOnlyOk_Error(QString::fromStdString(e.what()));

    }
}

void Student_win::on_ExitBtn_clicked()
{
    this->close();
}
