/*************************************************************************
【文件名】tchr_process.cpp
【功能模块和目的】教师业务流程类定义
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【版权信息】开发者范静涛(fanjingtao@tsinghua.edu.cn)声明放弃任何版权诉求，任何使用者可做出任何修改、用于任何目的
【更改记录】
    2020-05-19 由范静涛修改为信号机制通知界面更新
    2020-06-09 由邱亦文增添“随机语音提问”功能，能够进行随机开麦
    2020-06-09 由邱亦文增添“收集注意力”功能
    2020-06-12 由邱亦文完善“随机语音提问”功能，能够进行语音向其他人转播，并由教师端/同学端结束语音提问
    2020-06-15 由邱亦文增添“发题”功能，能够发送文本题目
    2020-06-16 由邱亦文完善“发题”功能，能够收到学生答案，并由教师端一键收题
    2020-06-16 由邱亦文增添注释
    2020-06-17 由邱亦文将线程退出等待时间由INFINITE改为 2000
    2020-06-20 由邱亦文增加“多选”--SaveMultiAnswerResult
*************************************************************************/
#include "tchr_process.h"
#include <string>
#include "user.h"
#include "student.h"
#include <QDebug>
#include "mymessagebox.h"
using namespace std;

/*************************************************************************
【函数名称】Teacher_Process::Teacher_Process
【函数功能】构造函数
【参数】入口参数，父对象指针，默认为nullptr
【返回值】构造函数不可有返回值
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 由范静涛删除了界面类控件指针，修改为信号机制通知界面更新
    2020-06-11 由邱亦文更新对注意力时长记录成员的初始化
    2020-06-13 由邱亦文更新对语音提问状态，音频播放对象的初始化
    2020-06-15 由邱亦文更新对发送题目状态，答题情况统计的初始化
*************************************************************************/
Teacher_Process::Teacher_Process(QObject* Parent)
    : QObject(Parent), IsInClass(m_bIsInClass),
      IsScreenSharing(m_bIsScreenSharing), IsVoiceSharing(m_bIsVoiceSharing),
      IsVoiceSharing2(m_bIsVoiceSharing2), IsAskingQues(m_bIsAskingQues) {
    //新建Server
    m_pServer = new Server();
    //新建录音对象
    m_pVoiceCapture = new Audio();
    //新建播放列表
    m_pVoicePlayer = new Audio();
    //更新为未在上课状态
    m_bIsInClass = false;
    //更新为未在屏幕共享状态
    m_bIsScreenSharing = false;
    //更新为未在语音直播状态
    m_bIsVoiceSharing = false;
    //更新为没有随机点名
    m_bIsVoiceSharing2 = false;
    //更新为没有发题
    m_bIsAskingQues = false;
    //初始化所有线程句柄为空
    m_hScreeShareThread = nullptr;
    m_hVoiceShareThread = nullptr;
    m_hMsgReceiveThread = nullptr;
    //初始化三个数组，其实不必要，怕后面写忘了，放在这里作提示
    m_Names.clear();
    m_Passwords.clear();
    m_ConnectionIdx.clear();
    m_AllFocusTimes.clear();
    m_QuestionData.clear();

    qDebug() << "TP: Created";
}

/*************************************************************************
【函数名称】Teacher_Process::~Teacher_Process
【函数功能】析构函数
【参数】无
【返回值】析构函数不可有返回值
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
*************************************************************************/
Teacher_Process::~Teacher_Process() {
    //下课
    EndClass();
    //释放动态开辟的对象
    delete m_pServer;
    m_pServer = nullptr;
    delete m_pVoiceCapture;
    m_pVoiceCapture = nullptr;
    delete m_pVoicePlayer;
    m_pVoicePlayer = nullptr;
}

/*************************************************************************
【函数名称】Teacher_Process::BeginClass
【函数功能】开始上课
【参数】无
【返回值】true为当前正在上课
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 由范静涛修改为信号机制通知界面更新
    2020-06-16 由邱亦文更新为默认状态下开始语音直播
*************************************************************************/
bool Teacher_Process::BeginClass() {
    //已是上课状态，什么都不做
    if (m_bIsInClass) {
        return m_bIsInClass;
    }
    //清空三个数组，相当于没有人登录了
    m_Names.clear();
    m_Passwords.clear();
    m_ConnectionIdx.clear();
    //所有学生对象下线，之前登录状态都不算
    User::OfflineAllStudents();
    //打开服务端
    qDebug() << "TP: open server";
    m_pServer->Open(3004);

    //更新状态为上课中
    m_bIsInClass = true;
    //创建并启动消息接收与处理线程
    m_hMsgReceiveThread =  CreateThread(nullptr, 0, MsgReceiveThread, (LPVOID)this, 0, nullptr);
    if (m_hMsgReceiveThread == nullptr) {
        qDebug() << "TP: failed to create a MsgRecv&Process thread";
        throw(runtime_error("Failed to create a MsgRecv&Process thread!"));
    }
    StartVoiceShare();
    //发送开始上课信号，要求界面更新
    qDebug() << "TP: class begin";
    emit ClassBegined();
    return m_bIsInClass;
}




/*************************************************************************
【函数名称】Teacher_Process::AboutEndClass
【函数功能】在下课之前，提前通知学生发送注意力消息, 并停止正在进行的一切活动
【参数】无
【返回值】true为成功通知
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-6-10
【更改记录】
    2020-05-19 由范静涛修改为信号机制通知界面更新
*************************************************************************/
bool Teacher_Process::AboutEndClass() {
    //停止可能正在的屏幕共享
    StopScreenShare();
    //停止可能正在的语音直播
    StopVoiceShare();
    //停止语音提问
    StopVoiceShare2();
    //把可能的题目收上来；
    CollectAnswer();
    //告诉学生要下课了
    return SendDismiss();
}


/*************************************************************************
【函数名称】Teacher_Process::EndClass
【函数功能】下课
【参数】无
【返回值】true为当前正在上课（虽然是下课功能，但返回值为真时正在上课）
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 由范静涛修改为信号机制通知界面更新
    2020-06-11 由邱亦文更新对注意力时长记录的清理
    2020-06-13 由邱亦文更新对语音提问状态，音频播放对象的清理
    2020-06-15 由邱亦文更新对发送题目状态，答题情况统计的清理
*************************************************************************/
bool Teacher_Process::EndClass() {
    //已下课，则检查消息接收与处理线程是否自主退出了
    if (!m_bIsInClass) {
        if(m_hMsgReceiveThread != nullptr) {
            qDebug() << "TP: clean MsgRecv&Process thread handle";
            CloseHandle(m_hMsgReceiveThread);
            m_hMsgReceiveThread = nullptr;
        }
        return m_bIsInClass;
    }

    //关闭服务端
    qDebug() << "TP: server close";

    m_pServer->Close();
    //更新未在上课状态
    m_bIsInClass = false;
    //等待消息接收和处理线程结束
    qDebug() << "TP: wait for MsgRecv&Process thread exit";
    WaitForSingleObject(m_hMsgReceiveThread, 2000);
    qDebug() << "TP: clean MsgRecv&Process thread handle";
    CloseHandle(m_hMsgReceiveThread);
    m_hMsgReceiveThread = nullptr;

    //所有人下线，重置状态
    m_Names.clear();
    m_Passwords.clear();
    m_ConnectionIdx.clear();
    m_AllFocusTimes.clear();
    qDebug() << "TP: offline all students";
    User::OfflineAllStudents();

    //发送下课信号，要求界面更新
    qDebug() << "TP: class end";
//    emit ClassEnded();

    return m_bIsInClass;
}

/*************************************************************************
【函数名称】Teacher_Process::StartScreenShare
【函数功能】开始屏幕共享
【参数】无
【返回值】true为当前正在屏幕共享
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
*************************************************************************/
bool Teacher_Process::StartScreenShare() {
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    if (m_bIsScreenSharing) {
        return m_bIsScreenSharing;
    }
    m_hScreeShareThread = CreateThread(nullptr, 0, ScreenShareThread, (LPVOID)this, 0, nullptr);
    if (m_hScreeShareThread == nullptr) {
        qDebug() << "TP: failed to create a new screen share thread";
        throw(runtime_error("Failed to create a new screen share thread!"));
    }
    m_bIsScreenSharing = true;
    qDebug() << "TP: start screen share";
    return m_bIsScreenSharing;
}

/*************************************************************************
【函数名称】Teacher_Process::StopScreenShare
【函数功能】停止屏幕共享
【参数】无
【返回值】true为当前正在屏幕共享
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-06-16 由邱亦文添加发送停止共享屏幕消息
*************************************************************************/
bool Teacher_Process::StopScreenShare() {
    if (!m_bIsScreenSharing) {
        return m_bIsScreenSharing;
    }
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    m_bIsScreenSharing = false;
    qDebug() << "TP: wait for screen share thread exit";
    WaitForSingleObject(m_hScreeShareThread, 2000);
    qDebug() << "TP: clean screen share thread handle";
    CloseHandle(m_hScreeShareThread);
    m_hScreeShareThread = nullptr;
    qDebug() << "TP: stop screen share";
    m_pServer->SendToAll(Message::MakeScreenStopMsg());
    return m_bIsScreenSharing;
}

/*************************************************************************
【函数名称】Teacher_Process::StartVoiceShare
【函数功能】开始语音直播
【参数】无
【返回值】true为当前正在语音直播
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
*************************************************************************/
bool Teacher_Process::StartVoiceShare() {
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    if (m_bIsVoiceSharing) {
        return m_bIsVoiceSharing;
    }
    m_hVoiceShareThread = CreateThread(nullptr, 0, VoiceShareThread, (LPVOID)this, 0, nullptr);
    if (m_hVoiceShareThread == nullptr) {
        qDebug() << "TP: failed to create a new voice share thread";
        throw(runtime_error("Failed to create a new voice share thread!"));
    }
    m_pVoiceCapture->StartRec();
    m_bIsVoiceSharing = true;
    qDebug() << "TP: start voice share";
    return m_bIsVoiceSharing;
}

/*************************************************************************
【函数名称】Teacher_Process::StopVoiceShare
【函数功能】停止语音直播
【参数】无
【返回值】true为当前正在语音直播
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
*************************************************************************/
bool Teacher_Process::StopVoiceShare() {
    if (!m_bIsVoiceSharing) {
        return m_bIsVoiceSharing;
    }
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    m_pVoiceCapture->StopRec();
    m_bIsVoiceSharing = false;
    qDebug() << "TP: wait for voice share thread exit";
    WaitForSingleObject(m_hVoiceShareThread, 2000);

    CloseHandle(m_hVoiceShareThread);
    m_hVoiceShareThread = nullptr;
    qDebug() << "TP: clean voice share thread handel";
    AudioFrame TempFrame;
    while(m_pVoiceCapture->RecordFramesCount() > 0) {
        m_pVoiceCapture->GetAndRemoveFirstRecorded(TempFrame);
    }
    qDebug() << "TP: clean voice share queue";
    qDebug() << "TP: stop voice share";
    return m_bIsVoiceSharing;
}



/*************************************************************************
【函数名称】Teacher_Process::StartVoiceShare2
【函数功能】开始语音随机提问
【参数】无
【返回值】true为当前正在语音提问
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-12
【更改记录】
*************************************************************************/
bool Teacher_Process::StartVoiceShare2() {
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    if (m_bIsVoiceSharing2) {
        return m_bIsVoiceSharing2;
    }
    //随便找个学生
    srand((unsigned)time(NULL));
    if (m_ConnectionIdx.size() == 0) {
        return false;
    }
    int temp = rand() % m_ConnectionIdx.size();
    m_pVoicePlayer->Clear();
    if (m_pServer->SendTo(m_ConnectionIdx[temp], Message::MakeRequestVoiceMsg())) {
        m_bIsVoiceSharing2 = true;
        emit CallingToStu(QString::fromStdString(GetNameofCnction(m_ConnectionIdx[temp])));
        qDebug() << "TP: asking to voice answer";
        m_pVoicePlayer->StartPlay();
        return m_bIsVoiceSharing2;
    }
    return false;
}


/*************************************************************************
【函数名称】Teacher_Process::StopVoiceShare2
【函数功能】停止语音随机提问,清理剩余语音消息
【参数】无
【返回值】true为当前正在语音提问
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-12
【更改记录】
*************************************************************************/
bool Teacher_Process::StopVoiceShare2() {
    if (!m_bIsVoiceSharing2) {
        return m_bIsVoiceSharing2;
    }
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    qDebug() << "TP: stop voicing answer";
    m_pServer->SendToAll(Message::MakeVoiceStopMsg());
    m_bIsVoiceSharing2 = false;
    m_pVoicePlayer->StopPlay();
    m_pVoicePlayer->Clear();
    return m_bIsVoiceSharing2;
}





/*************************************************************************
【函数名称】Teacher_Process::SendStrChat
【函数功能】群发消息
【参数】入口参数，消息内容QString
【返回值】是否发送成功
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
*************************************************************************/
bool Teacher_Process::SendStrChat(const QString& ChatStr) {
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    std::string ToSend = ChatStr.toStdString();
    Message Chat(MSG_STRING, (unsigned char*)ToSend.c_str(), ToSend.length());
    m_pServer->SendToAll(Chat);
    qDebug() << "TP: send to all message: " << ChatStr;
    return m_bIsInClass;
}



/*************************************************************************
【函数名称】Teacher_Process::SendVoiceExcept
【函数功能】转播学生发来的语音
【参数】入口参数，发来的（不需要转的）idx，对应开麦学生的connection编号
       入口参数，一截语音
【返回值】true为当前正在语音提问
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-14
【更改记录】
*************************************************************************/
bool Teacher_Process::SendVoiceExcept(unsigned int idx, Message& aVoice) {
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    m_pServer->SendToAllExcept(idx, aVoice);
    qDebug() << "TP: send voice to all except connection " << idx ;
    return m_bIsInClass;
}



/*************************************************************************
【函数名称】Teacher_Process::PlayVoiceFromStu
【函数功能】播放学生发来的语音消息
【参数】入口参数，发来的（不需要转的）idx，对应开麦学生的connection编号
       入口参数，一截语音
【返回值】true为当前正在语音提问且在播放
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-12
【更改记录】
*************************************************************************/
void Teacher_Process::PlayVoiceFromStu(unsigned int idx, Message& aVoice) {
    m_pVoicePlayer->AppendFrameToPlay(Message::ToAudioFrame(aVoice));
}



/*************************************************************************
【函数名称】Teacher_Process::SaveAnswerResult
【函数功能】存下学生的【姓名，答案，用时】
【参数】入口参数，发来的idx，对应答题学生的connection编号
       入口参数，一个答案消息
【返回值】无
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-15
【更改记录】
*************************************************************************/
void Teacher_Process::SaveSingleAnswerResult(unsigned int idx, Message& aAnswerMsg) {
    int tempRst = 0;
    char* timeUsed = new char[20] ;
    bool isAnswered = 0;
    Message::ToSingleAnswerResult(aAnswerMsg, &tempRst, timeUsed);
    //这个学生选了哪个选项
    for (int i = 1; i < 5;i++) {
        if (i == tempRst) {
            m_allChoiceResult[i]++;
            isAnswered = 1;
            break;
        }
    }
    if (!isAnswered) {
        m_allChoiceResult[0]++;
    }
    string strChoice;
    switch (tempRst) {
    case 0 : strChoice = "/";
        break;
    case 1 : strChoice = "A";
        break;
    case 2 : strChoice = "B";
        break;
    case 3 : strChoice = "C";
        break;
    case 4 : strChoice = "D";
        break;
    }
    m_QuestionData.push_back(GetNameofCnction(idx));
    m_QuestionData.push_back(strChoice);
    m_QuestionData.push_back(timeUsed);
    delete [] timeUsed;
    emit RecvAnswerResult();
}

/*************************************************************************
【函数名称】Teacher_Process::SaveMultiAnswerResult
【函数功能】存下学生的【姓名，答案，用时】
【参数】入口参数，发来的idx，对应答题学生的connection编号
       入口参数，一个答案消息
【返回值】无
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-20
【更改记录】
*************************************************************************/
void Teacher_Process::SaveMultiAnswerResult(unsigned int idx, Message& aAnswerMsg) {
    int tempRst[4] = {0};
    char* timeUsed = new char[20] ;
    bool isAnswered = 0;
    Message::ToMultiAnswerResult(aAnswerMsg, tempRst, timeUsed);
    //这个学生选了哪个选项,abcd轮流加一遍
    for (int i = 0; i < 4;i++) {
        m_allChoiceResult[i + 1]+= tempRst[i];
        if (tempRst[i] != 0) {
            isAnswered = 1;
        }
    }
    //选了什么
    string strChoice = {'\0'};
        //什么都没选
    if (!isAnswered) {
        m_allChoiceResult[0]++;
        strChoice = "/";
    }
        //选了ABCD
    else {
        for(int i = 0;i < 4;i++) {
            switch (i) {
            case 0 : if (tempRst[i]) {
                        strChoice += "A";
                    }
                break;
            case 1 : if (tempRst[i]) {
                        strChoice += "B";
                    }
                break;
            case 2 : if (tempRst[i]) {
                    strChoice += "C";
                }
                break;
            case 3 : if (tempRst[i]) {
                    strChoice += "D";
                }
                break;
            }

        }
    }
    m_QuestionData.push_back(GetNameofCnction(idx));
    m_QuestionData.push_back(strChoice);
    m_QuestionData.push_back(timeUsed);
    delete [] timeUsed;
    emit RecvAnswerResult();
}
/*************************************************************************
【函数名称】Teacher_Process::SendDismiss
【函数功能】发送马上下课的消息
【参数】无
【返回值】无
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-09
【更改记录】
*************************************************************************/
bool Teacher_Process::SendDismiss() {
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    Message msg = Message::MakeAboutToDismissMsg();
    m_pServer->SendToAll(msg);
    qDebug() << "TP: send to all message: class about to end";
    return m_bIsInClass;
}




/*************************************************************************
【函数名称】Teacher_Process::SendAQuestion
【函数功能】发送一个文本问题
【参数】均为入口参数，问题以及ABCD四个答案的文本
【返回值】true为正在上课
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-15
【更改记录】
*************************************************************************/
bool Teacher_Process::SendAQuestion(const Questiontype& type, const QString& Prob, const QString& ChoiceA, const QString& ChoiceB
                                    , const QString& ChoiceC, const QString& ChoiceD) {
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    //把作答情况置零
    for(int i = 0;i < 5;i++) {
        m_allChoiceResult[i] = 0;
    }
    m_QuestionData.clear();
    //发出问题
    Question ques(type, Prob.toStdString(), ChoiceA.toStdString(), ChoiceB.toStdString()
                  , ChoiceC.toStdString(), ChoiceD.toStdString());
    Message msg = ques.ToMessage();
    m_pServer->SendToAll(msg);
    qDebug() << "TP: send to all a question";
    m_bIsAskingQues = true;
    return m_bIsInClass;
}


/*************************************************************************
【函数名称】Teacher_Process::CollectAnswer
【函数功能】教师一键收题，结束发问题状态
【参数】无
【返回值】true为正在上课
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-16
【更改记录】
*************************************************************************/
bool Teacher_Process::CollectAnswer() {
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    if (!m_bIsAskingQues) {
        return true;
    }
    Message msg = Message::MakeCollectAnswMsg();
    m_pServer->SendToAll(msg);
    m_bIsAskingQues = false;
    qDebug() << "TP: send to all message: send your answer";
    return m_bIsInClass;
}




/*************************************************************************
【函数名称】Teacher_Process::GetNameofCnction
【函数功能】获得发来的 Connection对应的学生姓名
【参数】入口参数，connection编号
【返回值】string，学生姓名
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-13
【更改记录】
*************************************************************************/
string Teacher_Process::GetNameofCnction(unsigned int idx) const {
    for (unsigned int i = 0; i < m_Names.size(); i++) {
        //搜索idx对相应的数组记录，获取用户名
        if (m_ConnectionIdx[i] == idx) {
            return m_Names[i];
        }
    }
    return nullptr;
}

/*************************************************************************
【函数名称】Teacher_Process::AnyoneInClass
【函数功能】确认是否所有人都发送过注意力消息了
【参数】无
【返回值】false为所有人都已经下线（即都已经发送）
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-13
【更改记录】
*************************************************************************/
bool Teacher_Process::AnyoneInClass() const {
    return m_ConnectionIdx.size();
}



/*************************************************************************
【函数名称】Teacher_Process::GetHistoryName
【函数功能】获得每次存下的历史注意力消息
【参数】入口参数，第几个登录的学生
【返回值】string 学生姓名
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-13
【更改记录】
*************************************************************************/

string Teacher_Process::GetHistoryName(unsigned int idx) const {
    //m_AllFocusTimes.size()一定是个偶数
    if (2 * idx >= m_AllFocusTimes.size()) {
        return nullptr;
    }
    return m_AllFocusTimes[idx * 2];
}





/*************************************************************************
【函数名称】Teacher_Process::GetHistoryTime
【函数功能】获得每次存下的历史注意力消息
【参数】入口参数，第几个登录的学生
【返回值】string 学生注意力时间
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-13
【更改记录】
*************************************************************************/
string Teacher_Process::GetHistoryTime(unsigned int idx) const {
    if (2 * idx + 1 > m_AllFocusTimes.size()) {
        return nullptr;
    }
    return m_AllFocusTimes[idx * 2 + 1];
}




/*************************************************************************
【函数名称】Teacher_Process::GetFocusCount
【函数功能】获得总共收到了多少个注意力消息
【参数】无
【返回值】登录过的学生数量
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-13
【更改记录】
*************************************************************************/
unsigned int Teacher_Process::GetFocusCount() const {
    return m_AllFocusTimes.size() / 2;
}



/*************************************************************************
【函数名称】Teacher_Process::GetLastAnswer_name
【函数功能】获得最后一个作答情况_姓名
【参数】无
【返回值】学生姓名
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-16
【更改记录】
*************************************************************************/
string Teacher_Process::GetLastAnswer_name() const {
    unsigned int len = m_QuestionData.size();
    return m_QuestionData[len - 3];
}



/*************************************************************************
【函数名称】Teacher_Process::GetLastAnswer_choice
【函数功能】获得最后一个作答情况_选项
【参数】无
【返回值】选择了哪个选项，仅有 A,B,C,D,/,五种可能性
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-16
【更改记录】
*************************************************************************/
string Teacher_Process::GetLastAnswer_choice() const {
    unsigned int len = m_QuestionData.size();
    return m_QuestionData[len - 2];

}


/*************************************************************************
【函数名称】Teacher_Process::GetLastAnswer_time
【函数功能】获得最后一个作答情况_用时
【参数】无
【返回值】作答使用时间
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-16
【更改记录】
*************************************************************************/
string Teacher_Process::GetLastAnswer_time() const {
    unsigned int len = m_QuestionData.size();
    return m_QuestionData[len - 1];

}




/*************************************************************************
【函数名称】Teacher_Process::GetLastAnswer_choice
【函数功能】获得作答统计结果
【参数】选择了哪个选项，type是 0，1，2，3，4
                   分别对应 /, A, B, C, D（/代表未作答）
【返回值】选择了那个选项的数量
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-16
【更改记录】
*************************************************************************/
unsigned int Teacher_Process::GetAnswerCount(unsigned int type) const {
    return m_allChoiceResult[type];
}





/*************************************************************************
【函数名称】Teacher_Process::ScreenShareThread
【函数功能】屏幕共享线程函数
【参数】入口参数，Teacher_Process的指针
【返回值】无意义
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
*************************************************************************/
DWORD WINAPI Teacher_Process::ScreenShareThread(LPVOID lpParameter) {
    Teacher_Process* pProcess = (Teacher_Process*)lpParameter;
    Image TempImage;
    while (pProcess->IsScreenSharing) {
        //如果平均未发消息不足10，就抓屏幕，转换成jpg消息，从server群发出去
        if (pProcess->m_pServer->GetAvgToSendCount() <= 10) {
            CapScreenTo(TempImage);
            pProcess->m_pServer->SendToAll(Message::FromImageJpg(TempImage));
            //qDebug() << "TP: send to all screen";
        }
        //限定帧率：两次抓屏间隔不小于40ms（1/24秒）
        Sleep(40);
    }
    return 0;
}

/*************************************************************************
【函数名称】Teacher_Process::VoiceShareThread
【函数功能】语音线程函数
【参数】入口参数，Teacher_Process的指针
【返回值】无意义
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 范静涛优化了代码效率，每次都取出一个音频帧，不管是否发送，都
*************************************************************************/
DWORD WINAPI Teacher_Process::VoiceShareThread(LPVOID lpParameter) {
    Teacher_Process* pProcess = (Teacher_Process*)lpParameter;
    AudioFrame TempFrame;
    while (pProcess->IsVoiceSharing) {
        //取出一个录制好的音频帧
        if (pProcess->m_pVoiceCapture->GetAndRemoveFirstRecorded(TempFrame)) {
           // qDebug() << "TP: remove an audio frame";
            //如果平均未发消息不足10，就抓声音，转换成音频消息，从server群发出去
            if (pProcess->m_pServer->GetAvgToSendCount() <= 10) {
                pProcess->m_pServer->SendToAll(Message::FromAudio(TempFrame));
              //  qDebug() << "TP: send to all an audio frame";
            }
        }
    }
    return 0;
}



/*************************************************************************
【函数名称】Teacher_Process::MsgRecieveThread
【函数功能】消息接收和处理线程函数
【参数】入口参数，Teacher_Process的指针
【返回值】无意义
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 由范静涛修改为信号机制通知界面更新
    2020-06-10 由邱亦文增加对注意力时长消息的处理
    2020-06-13 由邱亦文修改对退出登录消息的处理
    2020-06-14 由邱亦文增加对语音消息的处理
    2020-06-14 由邱亦文增加对停止语音回答的处理
    2020-06-16 由邱亦文增加对答题情况的处理
*************************************************************************/
DWORD WINAPI Teacher_Process::MsgReceiveThread(LPVOID lpParameter) {
    Teacher_Process* pProcess = (Teacher_Process*)lpParameter;
    //临时消息pair，两个分量分别从哪个连接收到的消息、收到了什么消息
    pair<unsigned int, Message> TempPair;
    MessageType Type;
    while (pProcess->IsInClass) {
        //如果有待处理消息，则取出最早的一个
        if (pProcess->m_pServer->GetAndRemoveFirstMessage(TempPair)) {
            //获取消息类型
            Type = TempPair.second.Type();
            //如果是文字信息，就通知界面更新
            if (Type == MSG_STRING) {
                qDebug() << "TP: recieved s string from connection " << TempPair.first;
                emit pProcess->RecvChat(QString::fromStdString(Message::ToString(TempPair.second)));
            }
            //如果是用户名密码的登录消息
            else if (Type == MSG_ID_PSW ) {
                //这个学生已经登录了并正常通信中，则不允许重复登录；如果未登录过，但用户名密码不对，也是一样
                if (!pProcess->OnlineStudent(TempPair)) {
                    //断开连接
                    qDebug() << "TP: disconnect socket: " << TempPair.first;
                    pProcess->m_pServer->DisConnect(TempPair.first);
                }
            }
            //如果是退出消息，修改这个同学的登录状态
            else if (Type == MSG_EXIT) {
                unsigned int Idx = TempPair.first;
                string Name = pProcess->GetNameofCnction(Idx);
                emit pProcess->RecvStudentLogout(QString::fromStdString(Name));
                pProcess->OffLineStudent(TempPair.first);

            }

            //如果是语音，
            else if (Type == MSG_AUDIO) {
                qDebug() << "TP: received audio from connection " << TempPair.first;
                qDebug() << "TP: recieved audio frame message " << TempPair.second.Size << " bytes (to play to all)";
                pProcess->PlayVoiceFromStu(TempPair.first, TempPair.second);
                pProcess->SendVoiceExcept(TempPair.first, TempPair.second);
            //    emit pProcess->RecvVoiceFromStu(TempPair.first, TempPair.second);
            }
            //如果是时间,就保存到vector里面
            else if (Type == MSG_FOCUSTIME) {
                qDebug() << "TP: recieved time information from connection " << TempPair.first;
                //如果之前有过这个学生的数据消息，就加一次数据，不再 push_back两次了
                string Name = pProcess->GetNameofCnction(TempPair.first);
                bool isExisted = 0;
                for (unsigned int i = 0;i < pProcess->GetFocusCount();i++) {
                    //找到了这个学生的name
                     if (pProcess->GetHistoryName(i) == Name) {
                         //把原来的时间 +  '\n' + 新的时间
                        string tempstr = pProcess->GetHistoryTime(i);
                        tempstr = tempstr + '\n' + Message::ToTimeData(TempPair.second);
                        pProcess->m_AllFocusTimes[i * 2 + 1] = tempstr;
                        isExisted = 1;
                        break;
                    }
                }
                //没找到，就把名字和注意力都放在m_AllFocusTimes
                if (!isExisted) {
                    pProcess->m_AllFocusTimes.push_back(Name);
                    pProcess->m_AllFocusTimes.push_back(Message::ToTimeData(TempPair.second));
                }
            }
            else if (Type == MSG_STOP_AUDIO) {
                qDebug() << "TP: connection " << TempPair.first << " finish answering";
                pProcess->m_pVoicePlayer->StopPlay();
                pProcess->StopVoiceShare2();
                emit pProcess->RecvStopVoice();

            }
            else if (Type == MSG_A_ANSWER) {
                qDebug() << "TP: receive an answer result";
                pProcess->SaveSingleAnswerResult(TempPair.first, TempPair.second);
            }
            else if (Type == MSG_MULTI_ANSWER) {
                qDebug() << "TP: receive an answer result";
                pProcess->SaveMultiAnswerResult(TempPair.first, TempPair.second);
            }
        }
    }
    return 0;
}


//以下两个函数，不需要掌握
bool Teacher_Process::OnlineStudent(const pair<unsigned int, Message>& Pair) {
    string Name;
    string Password;
    //获得要登录的用户在哪个Connection
    unsigned int ConnectionIdx = Pair.first;
    //获得用户名密码
    Message::ToStudentNameAndPassword(Pair.second, Name, Password);
    //在用户列表中按用户名密码搜索用户
    qDebug() << "TP: " << QString::fromStdString(Name) << "is trying to login";
    const User* pCurrent = User::GetUser(Name, Password);
    //未找到此用户，返回false
    if (pCurrent == nullptr) {
        qDebug() << "TP: no user " << QString::fromStdString(Name);
        return false;
    }
    //找到用户则转换为Student指针
    Student* pStudent = const_cast<Student*>(dynamic_cast<const Student*>(pCurrent));
    int CurrentConnectionIdx = 0;
    //判定指定学生用户当前是否在线，如在线，在哪个connection
    if (pStudent->IsOnline(CurrentConnectionIdx)) {
        //学生在线，在CurrentConnectionIdx
        //判定CurrentConnectionIdx对应的连接是否处于通信状态
        if (m_pServer->IsConnected(CurrentConnectionIdx)) {
            qDebug() << "TP: " << QString::fromStdString(Name) << "has logined at " << CurrentConnectionIdx;
            //在通信状态，未断开，返回false，不允许重复登录
            return false;
        }
        //学生在线, 但connection已经断开了，更新
        else {
            //设定在线
            qDebug() << "TP: " << QString::fromStdString(Name) << " is reset to " << CurrentConnectionIdx;
            pStudent->Online(ConnectionIdx);
            //更新之前记录的ConnectionIdx
            for (unsigned int i = 0; i < m_Names.size(); i++) {
                if (m_Names[i] == Name && m_Passwords[i] == Password) {
                    m_ConnectionIdx[i] = ConnectionIdx;
                    break;
                }
            }
            //发出信号，让界面更新学生列表
            emit RecvStudentLogin(QString::fromStdString(Name));
            return true;
        }
    }
    //学生不在线
    else {
        //设定在吸纳
        qDebug() << "TP: " << QString::fromStdString(Name) << " login is ok. at " << CurrentConnectionIdx;
        pStudent->Online(ConnectionIdx);
        //记录用户名密码和ConnectionIdx到数组
        m_Names.push_back(Name);
        m_Passwords.push_back(Password);
        m_ConnectionIdx.push_back(ConnectionIdx);
        //发出信号，让界面更新学生列表
        emit RecvStudentLogin(QString::fromStdString(Name));
        return true;
    }
}

void Teacher_Process::OffLineStudent(unsigned int Idx) {
    //搜索idx号connection对应的学生对象
    for (unsigned int i = 0; i < m_Names.size(); i++) {
        //搜索idx对相应的数组记录，获取用户名密码
        if (m_ConnectionIdx[i] == Idx) {
            //按用户名密码找学生对象指针
            const User* pCurrent = User::GetUser(m_Names[i], m_Passwords[i]);
            if (pCurrent == nullptr) {
                return;
            }
            Student* pStudent = const_cast<Student*>(dynamic_cast<const Student*>(pCurrent));
            //下线/登出学生对象
            qDebug() << "TP: " << QString::fromStdString(m_Names[i]) << " logout";
            pStudent->Offline();
            //删连接记录
            m_ConnectionIdx.erase(m_ConnectionIdx.begin() + i);
            //发出信号，还要求更新界面显示
            emit RecvStudentLogout(QString::fromStdString(m_Names[i]));
            //删用户名记录
            m_Names.erase(m_Names.begin() + i);
            //删密码记录
            m_Passwords.erase(m_Passwords.begin() + i);
            break;
        }
    }
}
