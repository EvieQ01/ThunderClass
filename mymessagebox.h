/*************************************************************************
【文件名】MyMessageBox.h
【功能模块和目的】弹出消息框类声明
【开发者及日期】邱亦文 2020-06-14
【版权信息】
【更改记录】
    2020-06-18 由邱亦文增添注释
*************************************************************************/
#ifndef MYMESSAGEBOX_H
#define MYMESSAGEBOX_H

#include <QMessageBox>

/*************************************************************************
【类名】 MyMessageBox
【功能】弹出消息框类
【接口说明】
    构造函数 MyMessageBox
    析构函数~MyMessageBox
    静态函数 MessageOnlyOk_Information 生成仅有ok键的消息框（内容自定）
    静态函数 MessageOnlyOk_Error 生成仅有ok键的错误提示框（内容自定）
    静态函数 ChMessageOkCancel 生成有ok和cancel两个键的提示框（内容自定）

【开发者及日期】邱亦文 2020-06-13
【更改记录】
    2020-06-18 由邱亦文增添注释
*************************************************************************/

class MyMessageBox
{
public:
    MyMessageBox();
    ~MyMessageBox();
    //仅有ok键的消息框
    static void MessageOnlyOk_Information(QString info);
    //仅有ok键的错误提示框
    static void MessageOnlyOk_Error(QString info);
    //有ok和cancel两个键的提示框
    static int ChMessageOkCancel(QString info);
};



#endif // MYMESSAGEBOX_H
