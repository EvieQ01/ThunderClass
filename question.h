/*************************************************************************
【文件名】Question.h
【功能模块和目的】问题消息类的声明
【开发者及日期】邱亦文 2020-06-14
【版权信息】邱亦文
【更改记录】
    2020-06-15 由邱亦文增加 ToMessage函数，向通用消息类的转换
    2020-06-17 由邱亦文增加注释
    2020-06-20 由邱亦文增加多选题 的type
*************************************************************************/
#ifndef QUESTION_H
#define QUESTION_H
#include <string>

using namespace std;

class Message;

enum Questiontype {Q_SINGLE, Q_MUlTI, Q_UNKNOWN};
/*************************************************************************
【类名】Question
【功能】通用消息类的操作
【接口说明】
    构造函数 Question(const string& ques, const string& choiceA, const string& choiceB,
                                        const string& choiceC, const string& choiceD);

    析构函数 ~Question
    拷贝构造函数 Question(const Question& aQuestion);
    赋值运算符 Question& operator=(const Question& aQuestion);
    成员函数 GetProb() const 获得问题
    成员函数 GetChoiceA() const 读取选项A
    成员函数 GetChoiceB() const 读取选项B
    成员函数 GetChoiceC() const 读取选项C
    成员函数 GetChoiceD() const 读取选项D
    成员函数 ToMessage 产生一个用于发向学生的Message

【开发者及日期】邱亦文 2020-06-14

【更改记录】
    2020-06-15 由邱亦文增加 ToMessage函数，向通用消息类的转换
    2020-06-17 由邱亦文增加注释
    2020-06-20 由邱亦文增加多选题 的 m_type
*************************************************************************/
class Question
{
public:
    Question(const Questiontype& type, const string& ques, const string& choiceA, const string& choiceB,
                                            const string& choiceC, const string& choiceD);
    Question(const string& ques, const string& choiceA, const string& choiceB,
                                            const string& choiceC, const string& choiceD);

    ~Question();
    Question(const Question& aQuestion);
    Question& operator= (const Question& aQuestion);
    const string& GetProb() const;
    const string& GetChoiceA() const;
    const string& GetChoiceB() const;
    const string& GetChoiceC() const;
    const string& GetChoiceD() const;
    const Questiontype& mtype;
    Message ToMessage() const;

private:
    Questiontype m_type;
    string m_sProblem;
    string m_sChoiceA;
    string m_sChoiceB;
    string m_sChoiceC;
    string m_sChoiceD;
};

#endif // QUESTION_H
