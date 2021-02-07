/*************************************************************************
【文件名】Admin_Process.h
【功能模块和目的】管理员业务流程类声明
【开发者及日期】邱亦文 2020-06-13
【版权信息】
【更改记录】
    2020-06-18 由邱亦文增添注释
*************************************************************************/
#ifndef ADMIN_PROCESS_H
#define ADMIN_PROCESS_H

#include "user.h"
#include "admin.h"


/*************************************************************************
【类名】Admin_Process
【功能】管理员业务流程类
【接口说明】
    构造函数 Admin_Process
    析构函数~Admin_Process默认
    函数 AddUserToFile保存新的用户名密码
    函数 DeleteUserInFile删除用户
【开发者及日期】邱亦文 2020-06-13
【更改记录】
    2020-06-18 由邱亦文增添注释
*************************************************************************/

class Admin_Process
{
public:
    Admin_Process();
    //返回值为 0，不能添加管理员账号
    //返回值为 1，有重复
    //返回值为 2，ok
    int AddUserToFile(const string& Name, const string& Passwd, const string& Type);
    bool DeleteUserInFile(const string& Name, const string& Passwd, const string& Type);

};

#endif // ADMIN_PROCESS_H
