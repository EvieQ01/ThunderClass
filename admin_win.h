/*************************************************************************
【文件名】Admin_Win.h
【功能模块和目的】管理员界面类声明
【开发者及日期】邱亦文 2020-05-24
【版权信息】
【更改记录】
    2020-06-13 由邱亦文增添“增加用户”“删除用户”功能
    2020-06-18 由邱亦文增添注释
*************************************************************************/
#ifndef ADMIN_WIN_H
#define ADMIN_WIN_H


#include <QDialog>
#include <QComboBox>
#include "admin_process.h"

namespace Ui {
class Admin_Win;
}

/*************************************************************************
【类名】Admin_Win
【功能】管理员界面类
【接口说明】
    构造函数 Admin_Win
    析构函数~Admin_Win
    槽函数 on_AddUserBtn_clicked 增加用户
    槽函数 on_m_pConfirmBtn_clicked 确认增加用户
    槽函数 on_DeleUserBtn_clicked 删除用户
    槽函数 on_ConfimDelBtn_clicked 确认删除用户
    槽函数 on_CancelDelBtn_clicked 取消删除操作

【开发者及日期】邱亦文 2020-06-13
【更改记录】
    2020-06-18 由邱亦文增添注释
*************************************************************************/
class Admin_Win : public QDialog
{
    Q_OBJECT

public:
    explicit Admin_Win(QWidget *parent = nullptr);
    ~Admin_Win();

private slots:
    //实时更新允许操作的范围
    void TimerEvent();
    //增加用户
    void on_AddUserBtn_clicked();
    //确认增加用户
    void on_m_pConfirmBtn_clicked();
    //删除用户
    void on_DeleUserBtn_clicked();
    //确认删除用户
    void on_ConfimDelBtn_clicked();
    //取消删除操作
    void on_CancelDelBtn_clicked();

private:
    void paintEvent(QPaintEvent *event) override;
    void ShowAllinfo();
    Ui::Admin_Win *ui;
    Admin_Process* m_pProc;
    QTimer* m_pTimer;
    QPushButton* m_pConfirmBtn;
    QComboBox* m_pComBox;
    Admin* m_pAdmin;

};

#endif // ADMIN_WIN_H
