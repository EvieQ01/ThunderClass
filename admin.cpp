/*************************************************************************
【文件名】Admin.cpp
【功能模块和目的】管理员类定义
【开发者及日期】邱亦文 2020-06-13
【版权信息】
【更改记录】
    2020-06-18 由邱亦文增添注释
*************************************************************************/
#include "admin.h"



/*************************************************************************
【函数名称】Admin::Admin
【函数功能】构造函数，生成一个用户名，密码，账户类型均为Admin的账户
【参数】无
【返回值】构造函数不可有返回值
【开发者及日期】邱亦文 2020-06-13
【更改记录】
*************************************************************************/
Admin::Admin():User("Admin", "Admin", "Admin")
{

}




/*************************************************************************
【函数名称】Admin_Process::AddUserToFile
【函数功能】增加一个用户信息
【参数】均为入口参数，要增加的用户名，密码，用户类型
【返回值】返回值为 0，不能添加管理员账号
      //返回值为 1，有重复
      //返回值为 2，ok可以增加
【开发者及日期】邱亦文 2020-06-13
【更改记录】
*************************************************************************/
int Admin::AddUserToFile(const string& Name, const string& Passwd, const string& Type) {
    Admin* p;
    if (Name == "Admin") {
        return 0;
    }
    if (GetUser(Name) != nullptr) {
        return 1;
    }
    if (Type == "Teacher" || Type == "Student") {
        p = dynamic_cast<Admin* >(this);
        p->AddUser(Name, Passwd, Type);
        SaveToFile("Userinformation.txt");
        return 2;
    }
    else {
        return -1;
    }
}


/*************************************************************************
【函数名称】Admin_Process::DeleteUserInFile
【函数功能】删除一个用户信息
【参数】均为入口参数，要删除的用户名，密码，用户类型
【返回值】返回值为true为成功删除
【开发者及日期】邱亦文 2020-06-13
【更改记录】
*************************************************************************/
bool Admin::DeleteUserInFile(const string& Name, const string& Passwd, const string& Type) {
    Admin* p;
    p = dynamic_cast<Admin* >(this);
    if (p->DeleteUser(Name, Passwd, Type)) {
        SaveToFile("Userinformation.txt");
        return true;
    }
    return false;
}



