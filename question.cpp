/*************************************************************************
【文件名】Question.cpp
【功能模块和目的】问题消息类的定义
【开发者及日期】邱亦文 2020-06-14
【版权信息】邱亦文
【更改记录】
    2020-06-15 由邱亦文增加 ToMessage函数，向通用消息类的转换
    2020-06-17 由邱亦文增加注释
    2020-06-20 由邱亦文增加 多选题 的type
*************************************************************************/
#include "question.h"
#include "message.h"


/*************************************************************************
【函数名称】Question::Question
【函数功能】构造函数，构造一个问题
【参数】均为入口参数，题干和 ABCD四个答案,单选还是多选类型
【返回值】构造函数不可有返回值
【开发者及日期】邱亦文 2020-06-14
【更改记录】
*************************************************************************/
Question::Question(const Questiontype& type,const string& ques, const string& choiceA, const string& choiceB,
                   const string& choiceC, const string& choiceD)
    : mtype(m_type), m_type(type), m_sProblem(ques), m_sChoiceA(choiceA), m_sChoiceB(choiceB)
     , m_sChoiceC(choiceC), m_sChoiceD(choiceD)
{

}

/*************************************************************************
【函数名称】Question::Question
【函数功能】构造函数，构造一个问题(只在意内容，不在意type)
【参数】均为入口参数，问题和 ABCD四个答案(没有类型)
【返回值】构造函数不可有返回值
【开发者及日期】邱亦文 2020-06-20
【更改记录】
*************************************************************************/
Question::Question(const string& ques, const string& choiceA, const string& choiceB,
                   const string& choiceC, const string& choiceD)
    : mtype(m_type), m_type(Q_UNKNOWN), m_sProblem(ques), m_sChoiceA(choiceA), m_sChoiceB(choiceB)
     , m_sChoiceC(choiceC), m_sChoiceD(choiceD)
{

}

/*************************************************************************
【函数名称】Question::~Question
【函数功能】析构函数
【参数】无
【返回值】析构函数不可有返回值
【开发者及日期】邱亦文 2020-06-14
【更改记录】
*************************************************************************/
Question::~Question() {

}

/*************************************************************************
【函数名称】Question::Question
【函数功能】拷贝构造函数，构造一个问题
【参数】入口参数，一个问题
【返回值】构造函数不可有返回值
【开发者及日期】邱亦文 2020-06-14
【更改记录】
*************************************************************************/
Question::Question(const Question& aQuestion) : mtype(m_type){
    m_type = aQuestion.m_type;
    m_sProblem = aQuestion.m_sProblem;
    m_sChoiceA = aQuestion.m_sChoiceA;
    m_sChoiceB = aQuestion.m_sChoiceB;
    m_sChoiceC = aQuestion.m_sChoiceC;
    m_sChoiceD = aQuestion.m_sChoiceD;

}

/*************************************************************************
【函数名称】Question::operator=
【函数功能】Question类赋值运算符
【参数】入口参数，一个问题
【返回值】Question的引用
【开发者及日期】邱亦文 2020-06-14
【更改记录】
*************************************************************************/
Question& Question::operator= (const Question& aQuestion) {
    if (this != &aQuestion) {
        m_sProblem = aQuestion.m_sProblem;
        m_sChoiceA = aQuestion.m_sChoiceA;
        m_sChoiceB = aQuestion.m_sChoiceB;
        m_sChoiceC = aQuestion.m_sChoiceC;
        m_sChoiceD = aQuestion.m_sChoiceD;
    }
    return *this;
}


/*************************************************************************
【函数名称】Question::GetProb
【函数功能】获得Question的问题题干
【参数】无
【返回值】string
【开发者及日期】邱亦文 2020-06-14
【更改记录】
*************************************************************************/
const string& Question::GetProb() const {
    return m_sProblem;
}

/*************************************************************************
【函数名称】Question::GetChoiceA
【函数功能】获得Question的答案 A
【参数】无
【返回值】string
【开发者及日期】邱亦文 2020-06-14
【更改记录】
*************************************************************************/
const string& Question::GetChoiceA() const {
    return m_sChoiceA;
}

/*************************************************************************
【函数名称】Question::GetChoiceB
【函数功能】获得Question的答案 B
【参数】无
【返回值】string
【开发者及日期】邱亦文 2020-06-14
【更改记录】
*************************************************************************/
const string& Question::GetChoiceB() const {
    return m_sChoiceB;

}


/*************************************************************************
【函数名称】Question::GetChoiceC
【函数功能】获得Question的答案 C
【参数】无
【返回值】string
【开发者及日期】邱亦文 2020-06-14
【更改记录】
*************************************************************************/
const string& Question::GetChoiceC() const {
    return m_sChoiceC;
}


/*************************************************************************
【函数名称】Question::GetChoiceA
【函数功能】获得Question的答案 D
【参数】无
【返回值】string
【开发者及日期】邱亦文 2020-06-14
【更改记录】
*************************************************************************/
const string& Question::GetChoiceD() const {
    return m_sChoiceD;
}


/*************************************************************************
【函数名称】Question::ToMessage
【函数功能】将Question转化成一个通用 Message
【参数】无
【返回值】构造的Message
【开发者及日期】邱亦文 2020-06-14
【更改记录】
*************************************************************************/
Message Question::ToMessage() const {
    unsigned int ProbLen = m_sProblem.length();
    unsigned int ALen = m_sChoiceA.length();
    unsigned int BLen = m_sChoiceB.length();
    unsigned int CLen = m_sChoiceC.length();
    unsigned int DLen = m_sChoiceD.length();
    unsigned char* Buffer = new unsigned char[ProbLen + ALen + BLen + CLen + DLen + 5 * sizeof (unsigned int)];
    //problen, prob
    memcpy(Buffer, (char*)&ProbLen, sizeof(unsigned int));
    memcpy(Buffer + sizeof(unsigned int), m_sProblem.c_str(), ProbLen);
    //alen, ChoiceA
    memcpy(Buffer + sizeof(unsigned int) + ProbLen, (char*)&ALen, sizeof(unsigned int));
    memcpy(Buffer +  2 * sizeof(unsigned int) + ProbLen, m_sChoiceA.c_str(), ALen);

    //blen, ChoiceB
    memcpy(Buffer + sizeof(unsigned int) * 2 + ProbLen + ALen, (char*)&BLen, sizeof(unsigned int));
    memcpy(Buffer +  3 * sizeof(unsigned int) + ProbLen + ALen, m_sChoiceB.c_str(), BLen);

    //clen, ChoiceC
    memcpy(Buffer + sizeof(unsigned int) * 3 + ProbLen + ALen + BLen, (char*)&CLen, sizeof(unsigned int));
    memcpy(Buffer +  4 * sizeof(unsigned int) + ProbLen + BLen + ALen, m_sChoiceC.c_str(), CLen);

    //dlen, ChoiceD
    memcpy(Buffer + sizeof(unsigned int) * 4 + ProbLen + ALen + BLen + CLen, (char*)&DLen, sizeof(unsigned int));
    memcpy(Buffer +  5 * sizeof(unsigned int) + ProbLen+ BLen + ALen + CLen, m_sChoiceD.c_str(), DLen);

    if (m_type == Q_SINGLE) {
        Message ret(MSG_A_QUESTION, Buffer, ProbLen + ALen + BLen + CLen + DLen + 5 * sizeof (unsigned int));
        delete [] Buffer;
        return ret;
    }
    else if (m_type == Q_MUlTI) {
        Message ret(MSG_MULTI_QUESTION, Buffer, ProbLen + ALen + BLen + CLen + DLen + 5 * sizeof (unsigned int));
        delete [] Buffer;
        return ret;
    }
    else {
        throw invalid_argument("invalid question type");
    }
}




