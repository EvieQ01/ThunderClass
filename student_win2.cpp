/*************************************************************************
【文件名】 Student_win2.cpp
【功能模块和目的】学生上课界面类定义（学生的 window2）
【开发者及日期】邱亦文 2020-06-08
【版权信息】
【更改记录】
    2020-06-09 由邱亦文增添“随机语音提问”功能，能够被点名开麦
    2020-06-10 由邱亦文增添“发送注意力”功能实现
    2020-06-17 由邱亦文修改对收到图像的尺寸处理，以及收到屏幕共享停止的消息
    2020-06-17 由邱亦文完善界面
    2020-06-18 由邱亦文增添注释
*************************************************************************/

#include "student_win2.h"
#include "ui_student_win2.h"
#include <QImage>
#include <QPainter>

Student_win2::Student_win2(Stu_Process* const pProc,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Student_win2), m_pProc2(pProc)
{
    ui->setupUi(this);
    ui->StopVoicingBtn->hide();
    m_ptimer = new QTimer;                  //初始化倒计时时器
    m_ptimer->setInterval(1000);
    m_ptimer->start();
    m_pTimeRecordAll = new QTime(0, 0, 0);     //初始化时间(正向计时)
    m_pTimeRecordFocus = new QTime(0, 0, 0);   //初始化时间(正向计时)
    ui->TimelcdNumber->setDigitCount(8);
    ui->TimelcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->TimelcdNumber->display(m_pTimeRecordAll->toString("hh:mm:ss"));
    connect(m_pProc2, SIGNAL(RecvScreen(QImage)), this, SLOT(RecvScreen(QImage)));
    connect(m_pProc2, SIGNAL(RecvScreenStop()), this, SLOT(RecvScreenStop()));
    connect(m_pProc2, SIGNAL(ShouldSendFocusData()), this, SLOT(SendFocus()));
    connect(m_pProc2, SIGNAL(VoiceSending()), this, SLOT(VoiceSending()));
    connect(m_pProc2, SIGNAL(StopVoiceSending()), this, SLOT(StopVoiceSending()));
    connect(m_pProc2, SIGNAL(RecvAQues(QString, QString, QString, QString, QString)), this, SLOT(RecvAQues(QString, QString, QString, QString, QString)));
    connect(m_pProc2, SIGNAL(RecvMultiQues(QString, QString, QString, QString, QString)), this, SLOT(RecvMultiQues(QString, QString, QString, QString, QString)));
    connect(m_ptimer, SIGNAL(timeout()), this, SLOT(UpdateTime()));
}

void Student_win2::paintEvent(QPaintEvent *) {
    QPixmap pixmap = QPixmap(":/material/StuBack2.png").scaled(this->size());
    QPainter painter(this);
    painter.drawPixmap(this->rect(), pixmap);
    ui->btnExitClass->setIcon(QIcon("material/Stop.png"));
    ui->StopVoicingBtn->setIcon(QIcon("material/Exit.png"));
    ui->StopVoicingBtn->setIconSize(ui->StopVoicingBtn->size());
    ui->btnExitClass->setCursor(QCursor(Qt::PointingHandCursor));
    ui->StopVoicingBtn->setCursor(QCursor(Qt::PointingHandCursor));
}

Student_win2::~Student_win2()
{
    disconnect(m_pProc2, SIGNAL(RecvScreen(QImage)), this, SLOT(RecvScreen(QImage)));
    disconnect(m_ptimer,SIGNAL(timeout()),this,SLOT(UpdateTime()));
    delete m_ptimer;
    delete m_pTimeRecordAll;
    delete m_pTimeRecordFocus;
    delete ui;
}

void Student_win2::on_btnExitClass_clicked()
{
    if (!m_pProc2->SendFocusData(*m_pTimeRecordFocus, *m_pTimeRecordAll)) {
        MyMessageBox::MessageOnlyOk_Information("Exit Class. But your time information was lost");

    }
    m_pProc2->ExitClass();
    this->close();

}


void Student_win2::SendFocus() {
    if (m_pProc2->IsInClass) {
        if (m_pProc2->SendFocusData(*m_pTimeRecordFocus, *m_pTimeRecordAll)) {
            m_pProc2->ExitClass();
            this->close();
            MyMessageBox::MessageOnlyOk_Information("Class dismissed! ");
        }
    }
}



void Student_win2::UpdateTime() {
    //（每秒触发一次）自动自增1s，回显在屏幕上
    *m_pTimeRecordAll = m_pTimeRecordAll->addSecs(1);
    ui->TimelcdNumber->display(m_pTimeRecordAll->toString("hh:mm:ss"));
    //（聚焦在屏幕上）条件自增一秒，不显示
    if(this->isActiveWindow()) {
        *m_pTimeRecordFocus = m_pTimeRecordFocus->addSecs(1);
        this->setWindowTitle(m_sTitleName);
    }
    else {
        this->setWindowTitle("Tunderclass - offline");
    }
}


void Student_win2::VoiceSending() {
    ui->StopVoicingBtn->show();
    MyMessageBox::MessageOnlyOk_Information("You are the chosen one! Speak up!!!!");
    ui->VoiceStatusLbl->setText("Recording……");
}

void Student_win2::StopVoiceSending() {
    ui->StopVoicingBtn->hide();
    ui->VoiceStatusLbl->setText("You are not allowed to talk");
    MyMessageBox::MessageOnlyOk_Information("已全部静音");
}

//收到屏幕共享消息信号
void Student_win2::RecvScreen(QImage Img){
    QImage img;
    img = Img.scaled(ui->scrollArea->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation) ;
    //收到的图像尺寸和Label尺寸不一致，则调整Label尺寸
    if (ui->ShraredScreenArea->minimumSize() != img.size()) {
        ui->ShraredScreenArea->setMinimumSize(img.size());
    }
    //刷新Label显示
    ui->ShraredScreenArea->setPixmap(QPixmap::fromImage(img));
    ui->ShraredScreenArea->move(0, 0);
}


void Student_win2::on_StopVoicingBtn_clicked()
{
    m_pProc2->StopSendingVoice();
    ui->StopVoicingBtn->hide();
    ui->VoiceStatusLbl->setText("You are not allowed to talk");
}

void Student_win2::RecvAQues(QString prob, QString choiceA, QString choiceB, QString choiceC, QString choiceD) {
    m_pStudentWin3 = new student_ques_win3(m_pProc2);
    m_pStudentWin3->SetQuestion(prob, choiceA, choiceB, choiceC, choiceD);
    m_pStudentWin3->SetType(Q_SINGLE);
    m_pStudentWin3->exec();
    delete m_pStudentWin3;
}

void Student_win2::RecvMultiQues(QString prob, QString choiceA, QString choiceB, QString choiceC, QString choiceD) {
    m_pStudentWin3 = new student_ques_win3(m_pProc2);
    m_pStudentWin3->SetQuestion(prob, choiceA, choiceB, choiceC, choiceD);
    m_pStudentWin3->SetType(Q_MUlTI);
    m_pStudentWin3->exec();
    delete m_pStudentWin3;
}

void Student_win2::RecvScreenStop() {
    ui->ShraredScreenArea->move(0, 0);
    ui->ShraredScreenArea->setStyleSheet("background-color: transparent;color: rgb(255, 255, 255);");
    ui->ShraredScreenArea->setText("教师尚未共享屏幕...");
}


