/*************************************************************************
【文件名】MyMessageBox.cpp
【功能模块和目的】弹出消息框类定义
【开发者及日期】邱亦文 2020-06-14
【版权信息】
【更改记录】
    2020-06-18 由邱亦文增添注释
*************************************************************************/
#include "mymessagebox.h"
#include <QIcon>
MyMessageBox::MyMessageBox()
{
}
MyMessageBox::~MyMessageBox()
{

}
void MyMessageBox::MessageOnlyOk_Information(QString info)
{
    QMessageBox msg;
    msg.resize(600, 400);
    msg.setWindowIcon(QIcon("material/thunder.png"));
    msg.setWindowTitle("Notice");
    msg.setText(info);
    msg.setStyleSheet("font: 13pt; color:rgb(255, 255, 255);background-color:rgb(100, 100, 100)");
    msg.setIcon(QMessageBox::Information);
    msg.addButton("OK",QMessageBox::ActionRole);
    msg.exec();
}

void MyMessageBox::MessageOnlyOk_Error(QString info)
{
    QMessageBox msg;
    msg.resize(900, 600);
    msg.setWindowIcon(QIcon("material/thunder.png"));
    msg.setWindowTitle("Notice");
    msg.setText(info);
    msg.setStyleSheet("font: 13pt; color:rgb(255, 255, 255); background-color:rgb(100, 100, 100)");
    msg.setIcon(QMessageBox::Critical);
    msg.addButton("OK",QMessageBox::ActionRole);
    msg.exec();
}

int MyMessageBox::ChMessageOkCancel(QString info)
{
    QMessageBox msg;
    msg.resize(600, 400);
    msg.setWindowIcon(QIcon("material/thunder.png"));
    msg.setWindowTitle("Notice");
    msg.setText(info);
    msg.setStyleSheet("color:rgb(100, 100, 100);font: 13pt");
    msg.setIcon(QMessageBox::Information);
    msg.addButton("OK",QMessageBox::ActionRole);
    msg.addButton("Cancel",QMessageBox::ActionRole);
    return msg.exec();
}
