/*************************************************************************
【文件名】Admin.h
【功能模块和目的】管理员类的生成声明
【开发者及日期】邱亦文 2020-05-24
【版权信息】
【更改记录】
    2020-06-13 由邱亦文增加增删用户的函数
    2020-06-18 由邱亦文增加注释
*************************************************************************/
#ifndef ADMIN_H
#define ADMIN_H

#include "user.h"


/*************************************************************************
【类名】Admin
【功能】管理员类
【接口说明】
    构造函数 Admin();
    析构函数~Admin();默认
    函数 AddUserToFile 保存新的用户名密码
    函数 DeleteUserInFile 删除用户
【开发者及日期】邱亦文 2020-06-13
【更改记录】
    2020-06-18 由邱亦文增添注释
*************************************************************************/
class Admin : public User
{
public:

    //添加账户到文件
    //返回值为 0，不能添加管理员账号
    //返回值为 1，有重复
    //返回值为 2，ok
    int AddUserToFile(const string& Name, const string& Passwd, const string& Type);
    bool DeleteUserInFile(const string &Name, const string &Password, const string &Type);
private:
    friend class User;
    Admin();
};

#endif // ADMIN_H
