/*************************************************************************
【文件名】Teacher.h
【功能模块和目的】教师类的生成声明
【开发者及日期】邱亦文 2020-05-24
【版权信息】
【更改记录】2020-06-18 由邱亦文增加注释
*************************************************************************/
#ifndef TEACHER_H
#define TEACHER_H
#include "user.h"

/*************************************************************************
【类名】Teacher
【功能】用于生成教师账户（继承自User）
【接口说明】
    仅有构造函数Teacher；
【开发者及日期】邱亦文 2020-05-24
【更改记录】2020-06-18 由邱亦文增加注释
*************************************************************************/
class Teacher: public User
{
public:
    Teacher(const string& Name, const string& Password);
};

#endif // TEACHER_H