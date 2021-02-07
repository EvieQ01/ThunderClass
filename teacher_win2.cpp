/*************************************************************************
【文件名】 Teacher_win2.cpp
【功能模块和目的】教师上课界面类声明（教师的window2）
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
#include "teacher_win2.h"
#include "ui_teacher_win2.h"
#include <QDebug>
#include <QPainter>

Teacher_win2::Teacher_win2(Teacher_Process* const Proc2, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Teacher_win2), m_pProc2(Proc2)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowMinMaxButtonsHint);
    //基础设置
    ui->btnEndClass->setEnabled(true);
    ui->StuTWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->StuTWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
    ui->StuTWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条
    ui->StuTWidget->setRowCount(0);
    ui->btnEndClass->setIcon(QIcon("material/Exit.png"));
    ui->StopVoiceBtn->setIcon(QIcon("material/Stop.png"));
    ui->StopScreenBtn->setIcon(QIcon("material/Stop.png"));
    ui->StopRandomCallBtn->setIcon(QIcon("material/Stop.png"));
    //信号：学生登入登出信号
    connect(m_pProc2, SIGNAL(RecvStudentLogin(QString)), this, SLOT(RecvStudentLogin(QString)));
    connect(m_pProc2, SIGNAL(RecvStudentLogout(QString)), this, SLOT(RecvStudentLogout(QString)));
    connect(m_pProc2, SIGNAL(CallingToStu(QString)), this, SLOT(CallingToStu(QString)));
    //connect(m_pProc, SIGNAL(RecvChat(QString)), this, SLOT(RecvChat(QString)));
    m_pTimer = new QTimer();
    m_pTimer->setInterval(200);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(UpdateControlEnabled()));
    m_pTimer->start();
    //只显示关闭窗口按钮，没有放大缩小按钮
    //this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}

void Teacher_win2::paintEvent(QPaintEvent *) {
    QPixmap pixmap = QPixmap(":/material/TchrBack.png").scaled(this->size());
    QPainter painter(this);
    painter.drawPixmap(this->rect(), pixmap);

    ui->btnEndClass->setIcon(QIcon("material/Stop.png"));
    ui->RandomCallBtn->setIcon(QIcon("material/RandomVoice.png"));
    ui->VoiceShareBtn->setIcon(QIcon("material/MyVoiceShare.png"));
    ui->ScreenShareBtn->setIcon(QIcon("material/MyScreenShare.png"));
    ui->DiapatchQuesBtn->setIcon(QIcon("material/DispatchProb.png"));
    ui->StopVoiceBtn->setIcon(QIcon("material/Exit.png"));
    ui->StopScreenBtn->setIcon(QIcon("material/Exit.png"));
    ui->StopRandomCallBtn->setIcon(QIcon("material/Exit.png"));

    ui->VoiceShareBtn->setIconSize(ui->VoiceShareBtn->size());
    ui->ScreenShareBtn->setIconSize(ui->VoiceShareBtn->size());
    ui->DiapatchQuesBtn->setIconSize(ui->VoiceShareBtn->size());
    ui->RandomCallBtn->setIconSize(ui->VoiceShareBtn->size());

    ui->ScreenShareBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->DiapatchQuesBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->RandomCallBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->VoiceShareBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->StopVoiceBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->StopScreenBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->StopRandomCallBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->btnEndClass->setCursor(QCursor(Qt::PointingHandCursor));
}

Teacher_win2::~Teacher_win2()
{

    disconnect(m_pProc2, SIGNAL(RecvStudentLogin(QString)), this, SLOT(RecvStudentLogin(QString)));
    disconnect(m_pProc2, SIGNAL(RecvStudentLogout(QString)), this, SLOT(RecvStudentLogout(QString)));
 //   disconnect(m_pProc, SIGNAL(RecvChat(QString)), this, SLOT(RecvChat(QString)));

    delete ui;
}



//实时更新按钮可用情况
void Teacher_win2::UpdateControlEnabled() {
    ui->VoiceShareBtn->setEnabled(!m_pProc2->IsVoiceSharing);
    ui->ScreenShareBtn->setEnabled(!m_pProc2->IsScreenSharing);
    ui->RandomCallBtn->setEnabled(!m_pProc2->IsVoiceSharing2);
    ui->StopScreenBtn->setEnabled(m_pProc2->IsScreenSharing);
    ui->StopVoiceBtn->setEnabled(m_pProc2->IsVoiceSharing);
    ui->StopRandomCallBtn->setEnabled(m_pProc2->IsVoiceSharing2);
    ui->DiapatchQuesBtn->setEnabled(!m_pProc2->IsAskingQues);
//    qDebug() << "update btns.";
}



void Teacher_win2::on_ScreenShareBtn_clicked()
{
    try {
        if (m_pProc2->StartScreenShare()) {
            ui->ScreenShareBtn->setChecked(false);
       }

    } catch (runtime_error& e) {
        MyMessageBox::MessageOnlyOk_Error(QString::fromStdString(e.what()));
    }
}

void Teacher_win2::on_VoiceShareBtn_clicked()
{
    try {
        if (m_pProc2->StartVoiceShare()) {
            ui->VoiceShareBtn->setChecked(false);
        }

    } catch (runtime_error& e) {
        MyMessageBox::MessageOnlyOk_Error(QString::fromStdString(e.what()));
    }
    return;
}


void Teacher_win2::RecvStudentLogin(QString Name) {
    int row;
    QList<QTableWidgetItem*> itemVec = ui->StuTWidget->findItems(Name, Qt::MatchExactly);
    //如果之前登陆过，在表格里找到了，就不再加一行了，只把状态修改为online
    if (!itemVec.isEmpty()) {
        row = itemVec.at(0)->row();
        ui->StuTWidget->takeItem(row, 1);
        ui->StuTWidget->setItem(row , 1, new QTableWidgetItem(QIcon("material/id.png"),"Online"));
        return;
    }
    ui->StuTWidget->insertRow(0);
    ui->StuTWidget->setItem(0 , 0, new QTableWidgetItem(Name));
    ui->StuTWidget->setItem(0 , 1, new QTableWidgetItem(QIcon("material/id.png"),"Online"));
    return;
}

void Teacher_win2::RecvStudentLogout(QString Name) {
//如果学生登出，找一下，把状态修改为offline
    int row;
    QList<QTableWidgetItem*> itemVec = ui->StuTWidget->findItems(Name, Qt::MatchExactly);
    if (!itemVec.isEmpty()) {
        row = itemVec.at(0)->row();
        ui->StuTWidget->takeItem(row, 1);
        ui->StuTWidget->setItem(row , 1, new QTableWidgetItem("Offline"));
    }
    return;
}

void Teacher_win2::CallingToStu(QString Name) {
    MyMessageBox::MessageOnlyOk_Information(Name + " is called!");
    ui->whoIsTalkingLbl->setText(Name + " is called and talking...");
    ui->whoIsTalkingLbl->setStyleSheet("font:24px;color:rgb(250, 250, 250)");
}


void Teacher_win2::on_btnEndClass_clicked()
{
    m_pTeacherWin3 = new Teacher_exit_win3(m_pProc2, this);
    m_pProc2->AboutEndClass();
    this->setWindowOpacity(0.0);
    m_pTeacherWin3->exec();
    delete m_pTeacherWin3;
    m_pTeacherWin3 = nullptr;
    this->close();

}

void Teacher_win2::on_StopScreenBtn_clicked()
{
    m_pProc2->StopScreenShare();
}

void Teacher_win2::on_StopVoiceBtn_clicked()
{
    m_pProc2->StopVoiceShare();
}



void Teacher_win2::on_RandomCallBtn_clicked()
{
    if (!m_pProc2->StartVoiceShare2()) {
        MyMessageBox::MessageOnlyOk_Error("Connection Error: no one is in class");
    }

}

void Teacher_win2::RandomCallEnd()
{
    ui->whoIsTalkingLbl->setText("No one else is called");
    m_pProc2->StopVoiceShare2();
}

void Teacher_win2::on_StopRandomCallBtn_clicked()
{
    ui->whoIsTalkingLbl->setText("No one else is called");
    m_pProc2->StopVoiceShare2();
}

void Teacher_win2::on_DiapatchQuesBtn_clicked()
{
    m_pTeacherWin4 = new Teacher_ques_win4(m_pProc2);
    m_pTeacherWin4->exec();
    delete m_pTeacherWin4;
    m_pTeacherWin4 = nullptr;
}
