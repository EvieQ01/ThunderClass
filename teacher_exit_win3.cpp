/*************************************************************************
【文件名】 Teacher_exit_win3.cpp
【功能模块和目的】教师下课后界面类定义（教师的window3）
【开发者及日期】邱亦文 2020-06-10
【版权信息】
【更改记录】
    2020-06-17 由邱亦文完善界面
    2020-06-18 由邱亦文增添注释
*************************************************************************/
#include "teacher_exit_win3.h"
#include "ui_teacher_exit_win3.h"
#include <fstream>

Teacher_exit_win3::Teacher_exit_win3(Teacher_Process* pProc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Teacher_exit_win3), m_pProc3(pProc)
{
    ui->setupUi(this);
    this->setWindowTitle("Class Ended");
    QTimer::singleShot(30000, this, SLOT(MustUpdate()));
    m_pTimer = new QTimer;
    m_pTimer->setInterval(1000);
    m_pTimer->start();
    ui->FocusTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->FocusTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
    //ui->FocusTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(TimerEvent()));

}


//显示所有注意力消息
void Teacher_exit_win3::ShowFocusDataPiece() const{
    string name;
    string msg;
    for (unsigned int i = 0;i < m_pProc3->GetFocusCount();i++) {
        name = m_pProc3->GetHistoryName(i);
        msg = m_pProc3->GetHistoryTime(i);
        ui->FocusTableWidget->insertRow(0);
        ui->FocusTableWidget->setItem(0, 0, new QTableWidgetItem(QString::fromStdString(name)));
        ui->FocusTableWidget->setItem(0, 1, new QTableWidgetItem(QString::fromStdString(msg)));

    }
    ofstream Outfile("FocusData.txt");
    SaveFocusToFile(Outfile);
    Outfile.close();
    ui->FocusTableWidget->resizeColumnsToContents();
    ui->FocusTableWidget->resizeRowsToContents();
}

//存入文件
void Teacher_exit_win3::SaveFocusToFile(ofstream& OutFile) const {
    for (unsigned int i = 0;i < m_pProc3->GetFocusCount();i++) {
        OutFile << m_pProc3->GetHistoryName(i) << endl;
        OutFile << m_pProc3->GetHistoryTime(i) << endl;
    }
}

Teacher_exit_win3::~Teacher_exit_win3()
{
    delete m_pTimer;
    delete ui;
}


void Teacher_exit_win3::on_pushButton_clicked()
{
    this->close();
}


//check一下，当所有人都下线了以后，显示注意力消息
void Teacher_exit_win3::TimerEvent() {
    if (!m_pProc3->AnyoneInClass()) {
        ui->label->hide();
        ShowFocusDataPiece();
        m_pTimer->stop();
        m_pProc3->EndClass();
    }
}

//30s之后，就算没收齐，也要显示
void Teacher_exit_win3::MustUpdate() {
    if (m_pProc3->IsInClass) {
        ui->label->hide();
        ShowFocusDataPiece();
        m_pProc3->EndClass();
    }
}

