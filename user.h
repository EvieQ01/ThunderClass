/*************************************************************************
【文件名】User.h
【功能模块和目的】用户类操作声明
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-05-03
【版权信息】
【更改记录】
    2020-06-10 由邱亦文添加User的几个只读函数
    2020-06-21 由邱亦文增添注释
*************************************************************************/
#ifndef USER_H
#define USER_H

#include <string>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "message.h"

using namespace std;

/*************************************************************************
【类名】User
【功能】用于操作用户
【接口说明】
    析构函数~User();
    拷贝构造函数User(const User& anUser);
    赋值运算符User& operator=(const User& anUser);
    函数GetName返回用名;
    函数ToMessage将用户信息打包为一个消息
    函数TestPassword用于测试给定密码是否为用户密码
    静态函数LoadFromFile从文件读入一批用户
    静态函数SaveToFile将全部用户信息存入文件
    静态函数GetUser(const string& Name, const string& Password)按用户名密码搜索用户
    静态函数GetUser(const string& Name)按用户名搜索用户
    静态函数GetLoginedUser获得最后一次登录的用户指针
    静态函数OfflineAllStudents使得所有学生用户对象离线
    静态常引用UserCount表示用户总个数
    常引用Type表示用户类型字串
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-05-03
【更改记录】
    2020-06-10 由邱亦文添加User的几个只读函数
    2020-06-21 由邱亦文增添注释
*************************************************************************/
class User {
public:
    //析构函数~User();
    virtual ~User();
    //拷贝构造函数User(const User& anUser);
    User(const User& anUser);
    //赋值运算符User& operator=(const User& anUser);
    User& operator=(const User& anUser);
    //函数GetName返回用名;
    string GetName() const;
    //函数GetPassword返回密码;
    string GetPassword() const;
    //函数GetUserList返回用户列表
    static vector<User*> GetUserList();
    //函数ToMessage将用户信息打包为一个消息
    Message ToMessage() const;
    //函数TestPassword用于测试给定密码是否为用户密码
    bool TestPassword(const string& Password) const;
    //静态函数LoadFromFile从文件读入一批用户
    static void LoadFromFile(const string& FileName);
    //静态函数SaveToFile将全部用户信息存入文件
    static void SaveToFile(const string& FileName);
    //静态函数GetUser(const string& Name, const string& Password)按用户名密码搜索用户
    static const User* GetUser(const string& Name, const string& Password);
    //静态函数GetUser(const string& Name)按用户名搜索用户
    static const User* GetUser(const string& Name);
    //静态函数GetLoginedUser获得最后一次登录的用户指针
    static User* GetLoginedUser();
    //静态函数OfflineAllStudents使得所有学生用户对象离线
    static void OfflineAllStudents();
    //静态常引用UserCount表示用户总个数
    static const unsigned int& UserCount;
    //常引用Type表示用户类型字串
    const string& Type;
protected:
    //构造函数
    User(const string& Name, const string& Password, const string& Type);
    //构造函数
    User(ifstream& inFile);
    //添加User
    virtual bool AddUser(const string& Name, const string& Password, const string& Type);
    //删除User
    virtual bool DeleteUser(const string& Name, const string& Password, const string& Type);
private:
    //保存到文件流
    void SaveToFileStream(ofstream& OutFile) const;
    //用户对象总个数
    static unsigned int m_uUserCount;
    //全部用户的指针数组
    static vector<User*> m_UserList;
    //最后一个从登录的用户指针
    static User* m_LoginedUser;
    string m_sName;
    string m_sPassword;
    string m_sType;
};

#endif // USER_H
