/*************************************************************************
【文件名】 student_ques_win3.cpp
【功能模块和目的】学生答题界面类定义（学生的window3）
【开发者及日期】邱亦文 2020-06-16
【版权信息】
【更改记录】
    2020-06-17 由邱亦文完善界面
    2020-06-18 由邱亦文增添注释
*************************************************************************/

#include "student_ques_win3.h"
#include "ui_student_ques_win3.h"
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QPainter>
#include "mymessagebox.h"

student_ques_win3::student_ques_win3(Stu_Process* pProc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::student_ques_win3)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    m_pProc3 = pProc;
    m_pTime = new QTime(0, 0, 0);
    m_pTimer = new QTimer;
    m_pTimer->setInterval(500);
    m_pTimer->start();
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
    connect(m_pProc3, SIGNAL(RecvFinishChoosing()), this, SLOT(RecvFinishChoosing()));
    ui->ChoiceBtnGroup->setId(ui->ChoiceABtn, 1);
    ui->ChoiceBtnGroup->setId(ui->ChoiceBBtn, 2);
    ui->ChoiceBtnGroup->setId(ui->ChoiceCBtn, 3);
    ui->ChoiceBtnGroup->setId(ui->ChoiceDBtn, 4);

}

void student_ques_win3::paintEvent(QPaintEvent *) {
    QPixmap pixmap = QPixmap(":/material/QuesBack.png").scaled(this->size());
    QPainter painter(this);
    painter.drawPixmap(this->rect(), pixmap);
    ui->ConfirmAnswBtn->setIcon(QIcon("material/yes.png"));
    ui->ConfirmAnswBtn->setCursor(QCursor(Qt::PointingHandCursor));
}

student_ques_win3::~student_ques_win3()
{
    disconnect(m_pTimer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
    delete m_pTime;
    delete m_pTimer;
    delete ui;
}

void student_ques_win3::SetQuestion(QString prob, QString choiceA, QString choiceB
                                    , QString choiceC, QString choiceD) {
    ui->ProblemTxt->setText(prob);
    ui->ChoiceABtn->setText(choiceA);
    ui->ChoiceBBtn->setText(choiceB);
    ui->ChoiceCBtn->setText(choiceC);
    ui->ChoiceDBtn->setText(choiceD);

}

void student_ques_win3::SetType(Questiontype type) {
    m_type = type;
    if (type == Q_MUlTI) {
        ui->ChoiceBtnGroup->setExclusive(false);
    }
}

void student_ques_win3::on_ConfirmAnswBtn_clicked()
{
    if(ui->ChoiceBtnGroup->checkedId() == -1) {
        if (!MyMessageBox::ChMessageOkCancel("You haven't choose your answer yet. Are you sure to submit ?")) {
            m_pProc3->SendChoiceAndTime(0, *m_pTime);
        }
        //else {}  指的是不发送，重新作答，就messagebox消失掉，啥都不干
        this->close();
        return;
    }
    switch (m_type) {
    case Q_MUlTI:
        int answ[4];
        answ[0] = ui->ChoiceABtn->isChecked();
        answ[1] = ui->ChoiceBBtn->isChecked();
        answ[2] = ui->ChoiceCBtn->isChecked();
        answ[3] = ui->ChoiceDBtn->isChecked();
        m_pProc3->SendChoiceAndTime(answ, *m_pTime);
        this->close();
        break;
        //UNKNOWN类先默认是单选
    case Q_UNKNOWN:
    case Q_SINGLE:
        m_pProc3->SendChoiceAndTime(ui->ChoiceBtnGroup->checkedId(), *m_pTime);
        this->close();
    }
}

void student_ques_win3::TimerEvent() {
    *m_pTime = m_pTime->addSecs(1);    
}


void student_ques_win3::RecvFinishChoosing() {
    if (ui->ChoiceBtnGroup->checkedId() == 1 || ui->ChoiceBtnGroup->checkedId() == 2 || ui->ChoiceBtnGroup->checkedId() == 3
                                             || ui->ChoiceBtnGroup->checkedId() == 4) {
        m_pProc3->SendChoiceAndTime(ui->ChoiceBtnGroup->checkedId(), *m_pTime);
        this->close();
    }
    else {
        //发送白卷
        m_pProc3->SendChoiceAndTime(0, *m_pTime);
        MyMessageBox::MessageOnlyOk_Information("Time's up !");
        this->close();

    }
}
