/*************************************************************************
【文件名】 Teacher_win.cpp
【功能模块和目的】教师登入界面类定义（教师的window1）
【开发者及日期】邱亦文 2020-05-23
【版权信息】
【更改记录】
    2020-06-17 由邱亦文完善界面
    2020-06-18 由邱亦文增添异常处理
    2020-06-18 由邱亦文增添注释
*************************************************************************/
#include "teacher_win.h"
#include "ui_teacher_win.h"
#include <QPainter>

Teacher_win::Teacher_win(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Teacher_win)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    m_pProc = new Teacher_Process(this);

}


void Teacher_win::paintEvent(QPaintEvent *) {
    QPixmap pixmap = QPixmap(":/material/TchrBack0.png").scaled(this->size());
    QPainter painter(this);
    painter.drawPixmap(this->rect(), pixmap);
    ui->exitBtn->setIcon(QIcon("material/Stop.png"));
    ui->beginBtn->setIcon(QIcon("material/yes.png"));
    ui->beginBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->exitBtn->setCursor(QCursor(Qt::PointingHandCursor));

}
Teacher_win::~Teacher_win()
{
    //先disconnected，再释放业务流程类
    delete m_pProc;
    delete ui;
}

void Teacher_win::on_beginBtn_clicked()
{
    //MyMessageBox::MessageOnlyOk_Error("该用户没有使用权限");
    try {
        m_pProc->BeginClass();
    } catch (runtime_error& e) {
        MyMessageBox::MessageOnlyOk_Error(e.what());
        return;
    }
    this->setWindowOpacity(0.0);
    m_pTeacherwin2 = new Teacher_win2(m_pProc, this);
    m_pTeacherwin2->setWindowTitle(this->windowTitle());
    m_pTeacherwin2->exec();
    this->setWindowOpacity(1.0);
    delete m_pTeacherwin2;
    m_pTeacherwin2 = nullptr;
}

void Teacher_win::on_exitBtn_clicked()
{
    this->close();
}
