/*************************************************************************
【文件名】Admin_Win.cpp
【功能模块和目的】管理员界面类定义
【开发者及日期】邱亦文 2020-05-24
【版权信息】
【更改记录】
    2020-06-13 由邱亦文增添“增加用户”“删除用户”功能
    2020-06-18 由邱亦文增添注释
*************************************************************************/
#include "admin_win.h"
#include "ui_admin_win.h"
#include "user.h"
#include <qtimer.h>
#include <mymessagebox.h>
#include <QDebug>
#include <QPainter>

Admin_Win::Admin_Win(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Admin_Win)
{
    ui->setupUi(this);
    this->setWindowTitle("ThundrClass - Welcome Admin !");
    m_pProc = new Admin_Process();
    m_pConfirmBtn = new QPushButton;
    m_pTimer = new QTimer();
    m_pTimer->setInterval(500);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
    connect(m_pConfirmBtn, SIGNAL(clicked()), this, SLOT(on_m_pConfirmBtn_clicked()));
    m_pComBox = new QComboBox;
    m_pComBox->addItem("Teacher");
    m_pComBox->addItem("Student");

    //显示所有账户且不允许编辑
    ShowAllinfo();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //去掉水平滚动条
    ui->tableWidget->resizeColumnsToContents();
    ui->ConfimDelBtn->hide();
    ui->DelLabel->hide();
    ui->AddLabel->hide();
    ui->CancelDelBtn->hide();
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}


Admin_Win::~Admin_Win()
{
    disconnect(m_pTimer, SIGNAL(timeout()), this, SLOT(TimerEvent()));
    delete m_pProc;
    m_pProc = nullptr;
    delete m_pConfirmBtn;
    m_pConfirmBtn = nullptr;
    delete m_pComBox;
    m_pComBox = nullptr;
    delete ui;
}



void Admin_Win::paintEvent(QPaintEvent *) {
    QPixmap pixmap = QPixmap(":/material/AdminBack.png").scaled(this->size());
    QPainter painter(this);
    painter.drawPixmap(this->rect(), pixmap);
    ui->AddUserBtn->setIcon(QIcon("material/Add.png"));
    ui->DeleUserBtn->setIcon(QIcon("material/Delete.png"));
    m_pConfirmBtn->setIcon(QIcon("material/yes.png"));
    ui->ConfimDelBtn->setIcon(QIcon("material/yes.png"));
    ui->ConfimDelBtn->setIconSize(ui->ConfimDelBtn->size());
    ui->CancelDelBtn->setIcon(QIcon("material/Stop2.png"));
    ui->CancelDelBtn->setText("Cancel");
    ui->AddUserBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->DeleUserBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->ConfimDelBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->CancelDelBtn->setCursor(QCursor(Qt::PointingHandCursor));

}



void Admin_Win::ShowAllinfo(){
    User* puser;
    vector<User*> aUserList = User::GetUserList();
    unsigned int a = User::UserCount;
    ui->tableWidget->setRowCount(a - 1);
  //  qDebug() << a;
    QString qstr;
    for (unsigned int row = 1;row < a;row++) {
        puser = aUserList[row];
        qstr = QString::fromStdString(puser->GetName());

        ui->tableWidget->setItem(row - 1, 0, new QTableWidgetItem(qstr));
        qstr = QString::fromStdString(puser->GetPassword());

        ui->tableWidget->setItem(row - 1, 1, new QTableWidgetItem(qstr));
        qstr = QString::fromStdString(puser->Type);

        ui->tableWidget->setItem(row - 1, 2, new QTableWidgetItem(qstr));

    }
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);//表头字体居中
    ui->tableWidget->resizeColumnsToContents();

}
void Admin_Win::TimerEvent(){
    //鼠标点击其他位置，则取消操作
    if(ui->tableWidget->currentRow() != 0) {
       this->setWindowTitle("Thunderclass - Admin");
       ui->AddLabel->hide();
       m_pTimer->stop();
       ui->tableWidget->removeCellWidget(0, 2);
       ui->tableWidget->removeCellWidget(0, 3);
       ui->tableWidget->removeRow(0);
       ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
       ui->AddUserBtn->setEnabled(true);
       ui->DeleUserBtn->setEnabled(true);
       m_pConfirmBtn = new QPushButton;
       m_pConfirmBtn->setIcon(QIcon("yes.png"));
       connect(m_pConfirmBtn, SIGNAL(clicked()), this, SLOT(on_m_pConfirmBtn_clicked()));
       m_pComBox = new QComboBox;
       m_pComBox->addItem("Teacher");
       m_pComBox->addItem("Student");

    }
    //三个指针都不为空
    else if (ui->tableWidget->item(0, 0) != nullptr && ui->tableWidget->item(0, 1) != nullptr){
        //三个单元格都写了内容
        if (ui->tableWidget->item(0, 0)->text() != "" && ui->tableWidget->item(0, 1)->text() != "") {
            this->m_pConfirmBtn->setEnabled(true);
            m_pConfirmBtn->setCursor(QCursor(Qt::PointingHandCursor));

        }
    }
}


void Admin_Win::on_AddUserBtn_clicked()
{
    this->setWindowTitle("Thunderclass - Admin - 添加用户");
    ui->AddLabel->show();
    ui->tableWidget->insertRow(0);
    ui->tableWidget->setCellWidget(0, 3, m_pConfirmBtn);
    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableWidget->setCellWidget(0, 2, m_pComBox);
    //暂时不许再添加了
    ui->AddUserBtn->setEnabled(false);
    ui->DeleUserBtn->setEnabled(false);
    //开始让管理员编辑第 0行，不允许点击确认，除非xxx（见 TimerEvent()）
    ui->tableWidget->setCurrentCell(0, 0);
    m_pConfirmBtn->setEnabled(false);
    m_pTimer->start();

}




void Admin_Win::on_m_pConfirmBtn_clicked(){

    int result = m_pProc->AddUserToFile(ui->tableWidget->item(0, 0)->text().toStdString()
                                        , ui->tableWidget->item(0, 1)->text().toStdString()
                                        , m_pComBox->currentText().toStdString());
    MyMessageBox msgBox;
    if (result == 0) {
        msgBox.MessageOnlyOk_Error("Cannot add 'Admin' account! ");
        ui->tableWidget->setCurrentCell(0, 0);
    }
    if (result == 1) {
        msgBox.MessageOnlyOk_Error("Acount '" + ui->tableWidget->item(0, 0)->text()+ "' already existed !");
        ui->tableWidget->setCurrentCell(0, 0);
    }
    if (result == 2) {
        QString type = m_pComBox->currentText();
        msgBox.MessageOnlyOk_Information("Successfully add acount '" + ui->tableWidget->item(0, 0)->text()+" '!");
        m_pTimer->stop();
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableWidget->removeCellWidget(0, 3);
        m_pConfirmBtn = new QPushButton;
        m_pConfirmBtn->setIcon(QIcon("yes.png"));
        ui->tableWidget->removeCellWidget(0, 2);
        m_pComBox = new QComboBox;
        m_pComBox->addItem("Teacher");
        m_pComBox->addItem("Student");
        ui->tableWidget->setItem(0, 2, new QTableWidgetItem(type));
        ui->AddUserBtn->setEnabled(true);
        ui->DeleUserBtn->setEnabled(true);
        ui->tableWidget->resizeColumnsToContents();
        this->setWindowTitle("Thunderclass - Admin");
        ui->AddLabel->hide();
    }
}


void Admin_Win::on_DeleUserBtn_clicked()
{
    ui->tableWidget->setCurrentCell(0, 0);
    ui->AddUserBtn->setEnabled(false);
    ui->DeleUserBtn->setEnabled(false);
    ui->ConfimDelBtn->show();
    ui->DelLabel->show();
    ui->CancelDelBtn->show();
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void Admin_Win::on_ConfimDelBtn_clicked()
{
    int row = ui->tableWidget->currentRow();
    string name = ui->tableWidget->item(row, 0)->text().toStdString();
    string passwd = ui->tableWidget->item(row, 1)->text().toStdString();
    string type = ui->tableWidget->item(row, 2)->text().toStdString();
    if (m_pProc->DeleteUserInFile(name, passwd, type)) {
        ui->DeleUserBtn->setEnabled(true);
        ui->AddUserBtn->setEnabled(true);
        ui->ConfimDelBtn->hide();
        ui->DelLabel->hide();
        ui->CancelDelBtn->hide();
        ui->tableWidget->resizeColumnsToContents();
        MyMessageBox::MessageOnlyOk_Information("Successfull delete account '"
                                                + ui->tableWidget->item(row, 0)->text() + " ' !");
        ui->tableWidget->removeRow(row);

    }

}

void Admin_Win::on_CancelDelBtn_clicked()
{
    ui->DeleUserBtn->setEnabled(true);
    ui->AddUserBtn->setEnabled(true);
    ui->ConfimDelBtn->hide();
    ui->DelLabel->hide();
    ui->CancelDelBtn->hide();
}
