/*************************************************************************
【文件名】LoginProcess.h
【功能模块和目的】登录业务流程类声明
【开发者及日期】邱亦文 2020-05-15
【版权信息】
【更改记录】
    2020-06-18 由邱亦文增添注释
    2020-06-21 由邱亦文增添三次登录失败，退出系统
*************************************************************************/
#include <vector>
#include <string>
#include <QString>

#ifndef LOGIN_PROCESS_H
#define LOGIN_PROCESS_H

using namespace std;

//登录结果
//NONE为用户不存在，A为Admin，T为Teacher，S为Student, LOG_FAIL为三次登录失败
enum Result {NONE, A, T, S, LOG_FAIL};

/*************************************************************************
【类名】LoginProcess
【功能】管理员业务流程类
【接口说明】
    构造函数 LoginProcess
    析构函数~LoginProcess默认
    函数 Login执行登录
【开发者及日期】邱亦文 2020-05-15
【更改记录】
    2020-06-18 由邱亦文增添注释
    2020-06-21 由邱亦文增添三次登录失败，退出系统
*************************************************************************/
class LoginProcess
{
    public:
        LoginProcess();
        Result Login(const QString& UserName, const QString& Password);
    private:
        //登录次数
        unsigned int m_iTimes = 0;
};

#endif // LOGIN_PROCESS_H
