/*************************************************************************
【文件名】 teacher_ques_win4.cpp
【功能模块和目的】教师提问界面类定义（教师的window4）
【开发者及日期】邱亦文 2020-06-15
【版权信息】
【更改记录】
    2020-06-20 由邱亦文增添注释
*************************************************************************/
#include "teacher_ques_win4.h"
#include "ui_teacher_ques_win4.h"
#include <QCloseEvent>
#include <QPainter>
Teacher_ques_win4::Teacher_ques_win4(Teacher_Process* aProc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Teacher_ques_win4)
{
    ui->setupUi(this);
    m_pProc4 = aProc;
    this->setWindowTitle("Thunderclass - Please enter your question");
    m_pTimer = new QTimer;
    m_pTimer->setInterval(500);
    m_pTimer->start();
    m_pTeacherWin5 = new Teacher_quesResult_win5(m_pProc4);
    m_pTeacherWin5->hide();
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(UpdateControlEnabled()));

}


void Teacher_ques_win4::paintEvent(QPaintEvent *) {
    QPixmap pixmap = QPixmap(":/material/QuesBack.png").scaled(this->size());
    QPainter painter(this);
    painter.drawPixmap(this->rect(), pixmap);
    ui->SendQuesBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->CollectAnswerBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->ShowInfoBtn->setCursor(QCursor(Qt::PointingHandCursor));
}



Teacher_ques_win4::~Teacher_ques_win4()
{
    disconnect(m_pTimer, SIGNAL(timeout()), this, SLOT(UpdateControlEnabled()));
    delete m_pTeacherWin5;
    delete m_pTimer;
    m_pTeacherWin5 = nullptr;
    m_pTimer = nullptr;
    delete ui;
}


void Teacher_ques_win4::UpdateControlEnabled() {
    if (!m_pProc4->IsAskingQues && ui->ProblemEdit->text() != "" && ui->ChoiceAedit->text() != "" && ui->ChoiceBedit->text() != ""
                                      && ui->ChoiceCedit->text() != "" && ui->ChoiceDedit->text() != "") {
        ui->SendQuesBtn->setEnabled(true);
    }
    else {
        ui->SendQuesBtn->setEnabled(false);
    }
}



void Teacher_ques_win4::on_SendQuesBtn_clicked() {

    Questiontype temp = ui->MultipleBtn->isChecked() ? Q_MUlTI : Q_SINGLE;
    if (m_pProc4->SendAQuestion(temp, ui->ProblemEdit->text(), ui->ChoiceAedit->text(), ui->ChoiceBedit->text()
                                                    ,ui->ChoiceCedit->text(), ui->ChoiceDedit->text())) {
        MyMessageBox::MessageOnlyOk_Information("Question sent successfully!");
    }
    else {
        MyMessageBox::MessageOnlyOk_Information("Failed to send a question !");
    }
}

void Teacher_ques_win4::on_CollectAnswerBtn_clicked()
{
    m_pProc4->CollectAnswer();
    this->hide();
    m_pTeacherWin5->exec();
}

void Teacher_ques_win4::on_ShowInfoBtn_clicked()
{
    m_pTeacherWin5->exec();
    if (!m_pProc4->IsAskingQues) {
        this->close();
    }
}

void Teacher_ques_win4::closeEvent (QCloseEvent* e) {
    //没收题之前不许关
    if (m_pProc4->IsAskingQues) {
        e->ignore();
        MyMessageBox::MessageOnlyOk_Information("Please collect answer before leaving !");

    }
}

