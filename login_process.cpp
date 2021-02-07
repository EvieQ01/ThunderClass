/*************************************************************************
【文件名】LoginProcess.cpp
【功能模块和目的】登录业务流程类定义
【开发者及日期】邱亦文 2020-05-15
【版权信息】
【更改记录】
    2020-06-18 由邱亦文增添注释
    2020-06-21 由邱亦文增添三次登录失败，退出系统
*************************************************************************/
#include "login_process.h"
#include "user.h"
using namespace std;


/*************************************************************************
【函数名称】LoginProcess::LoginProcess
【函数功能】构造函数，加载全部用户信息，由于后续比对
【参数】无
【返回值】构造函数不可有返回值
【开发者及日期】邱亦文 2020-05-15
【更改记录】
    2020-06-18 由邱亦文增添注释
*************************************************************************/
LoginProcess::LoginProcess()
{
    User::LoadFromFile("UserInformation.txt");
}


/*************************************************************************
【函数名称】LoginProcess::Login
【函数功能】尝试进行登录操作
【参数】入口参数，用户名和密码
【返回值】登录结果的账户类型 （详见 enum对象）
【开发者及日期】邱亦文 2020-05-15
【更改记录】
    2020-06-18 由邱亦文增添注释
    2020-06-21 由邱亦文增添“已经尝试登录次数”记录
*************************************************************************/
Result LoginProcess::Login(const QString& UserName, const QString& Password){
    const User* pCurrentUser = User::GetUser(UserName.toStdString(), Password.toStdString());
    if (pCurrentUser == nullptr) {
        m_iTimes++;
        if (m_iTimes == 3) {
            return LOG_FAIL;
        }
        return NONE;
    }
    if (pCurrentUser->Type == "Admin") {
        m_iTimes = 0;
        return A;
    }
    else if (pCurrentUser->Type == "Teacher") {
        m_iTimes = 0;
        return T;
    }
    else {
        m_iTimes = 0;
        return S;
    }
}
