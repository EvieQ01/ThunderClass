/*************************************************************************
【文件名】tchr_process.h
【功能模块和目的】教师业务流程类声明
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

#ifndef TEACHER_PROCESS_H
#define TEACHER_PROCESS_H

#include <QObject>
#include "Server.h"
#include "message.h"
#include "audio.h"
#include <vector>
#include <utility>
#include <QComboBox>
#include <QLineEdit>

using namespace std;


/*************************************************************************
Teacher_Process
【功能】学生上课业务流程类
【接口说明】
    构造函数 TeacherProcess
    析构函数~TeacherProcess
    函数 BeginClass上课
    函数 AboutEndClass即将下课
    函数 EndClass真正下课
    函数 StartScreenShare开始屏幕共享
    函数 StopScreenShare停止屏幕共享
    函数 StartVoiceShare开始语音直播
    函数 StopVoiceShare停止语音直播
    函数 StartVoiceShare2开始从学生随机语音提问
    函数 StopVoiceShare2停止随机提问;
    函数 SendStrChat群发文本信息
    函数 SendDismiss快下课了的消息;
    函数 SendAQuestion发送一道题目
    函数 GetHistoryName获得第idx个登录的同学姓名
    函数 GetHistoryTime获得第idx个登录的同学注意力时长
    函数 GetLastAnswer_name获得最后一个作答人姓名（用于更新界面）
    函数 GetLastAnswer_choice获得最后一个作答选择（用于更新界面）
    函数 GetLastAnswer_time获得最后一个作答用时（用于更新界面）
    函数 GetFocusCount()获得总登录人数
    函数 GetAnswerCount 获得某个选项的总作答数 0对应未作答，1对应 A，2对应 B，3对应 C,4对应 D

    成员 bool& IsInClass;
    成员 bool& IsScreenSharing;
    成员 bool& IsVoiceSharing;
    成员 bool& IsVoiceSharing2;
    成员 bool& IsAskingQues;

    信号
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 由范静涛修改为从QObject继承，并添加了使用信号更新界面的机制
    2020-06-09 由邱亦文增添“随机语音提问”功能，能够进行随机开麦
    2020-06-09 由邱亦文增添“收集注意力”功能
    2020-06-12 由邱亦文完善“随机语音提问”功能，能够进行语音向其他人转播，并由教师端/同学端结束语音提问
    2020-06-15 由邱亦文增添“发题”功能，能够发送文本题目
    2020-06-16 由邱亦文完善“发题”功能，能够收到学生答案，并由教师端一键收题
    2020-06-16 由邱亦文增添注释
    2020-06-20 由邱亦文增加“多选”--SaveMultiAnswerResult
*************************************************************************/
class Teacher_Process : public QObject{
    Q_OBJECT
public:
    //构造函数
    explicit Teacher_Process(QObject* Parent = nullptr);

    //析构函数
    ~Teacher_Process();

    //禁用拷贝构造函数
    Teacher_Process(const Teacher_Process&) = delete;

    //禁用赋值运算符
    Teacher_Process& operator=(const Teacher_Process&) = delete;

    //上课
    bool BeginClass();

    //即将下课
    bool AboutEndClass();

    //真正下课
    bool EndClass();

    //开始屏幕共享
    bool StartScreenShare();

    //停止屏幕共享
    bool StopScreenShare();

    //开始语音直播
    bool StartVoiceShare();

    //停止语音直播
    bool StopVoiceShare();

    //开始从学生随机语音提问
    bool StartVoiceShare2();

    //停止随机提问
    bool StopVoiceShare2();

    //群发文字
    bool SendStrChat(const QString& ChatStr);

    //快下课了的消息
    bool SendDismiss();

    //发送一道题目
    bool SendAQuestion(const Questiontype& type,const QString& Prob, const QString& ChoiceA, const QString& ChoiceB
                                          , const QString& ChoiceC, const QString& ChoiceD);

    //收题
    bool CollectAnswer();

    //在不在上课
    const bool& IsInClass;

    //在不在共享屏幕
    const bool& IsScreenSharing;

    //在不在语音直播
    const bool& IsVoiceSharing;

    //在不在语音提问
    const bool& IsVoiceSharing2;

    //在不在发题
    const bool& IsAskingQues;

    //第几个connection对应的学生姓名
    string GetNameofCnction(unsigned int idx) const;

    //是否所有人都发送过注意力消息，断开连接了
    bool AnyoneInClass() const;

    //获得第idx个登录的同学姓名
    string GetHistoryName(unsigned int idx) const;

    //获得第idx个登录的同学注意力时长
    string GetHistoryTime(unsigned int idx) const;

    //获得最后一个作答人姓名（用于更新界面）
    string GetLastAnswer_name() const;

    //获得最后一个作答选择（用于更新界面）
    string GetLastAnswer_choice() const;

    //获得最后一个作答用时（用于更新界面）
    string GetLastAnswer_time() const;

    //获得总登录人数
    unsigned int GetFocusCount() const;

    //获得某个选项的总作答数 0对应未作答，1对应 A，2对应 B，3对应 C,4对应 D
    unsigned int GetAnswerCount(unsigned int type) const;


signals:
    void ClassBegined();
    void ClassEnded();
    void RecvBeginClass();
    void RecvStudentLogin(QString Name);
    void RecvStudentLogout(QString Name);
    void RecvChat(QString Msg);
    void RecvFocusData(QString Name, QString msg);
    void RecvStopVoice();
    void RecvVoiceFromStu(unsigned int idx, Message& aVoice);
    void CallingToStu(QString Name);
    void RecvAnswerResult();

private :
    //转发出去
    bool SendVoiceExcept(unsigned int idx, Message& aVoice);
    //自己放
    void PlayVoiceFromStu(unsigned int idx, Message& aVoice);

    //存一下作答记录(单选)
    void SaveSingleAnswerResult(unsigned int idx, Message& aAnswerMsg);

    //存一下作答记录(多选)2020-06-20 邱亦文添加
    void SaveMultiAnswerResult(unsigned int idx, Message& aAnswerMsg);

    //上线一个学生
    bool OnlineStudent(const pair<unsigned int, Message>& Pair);

    //下线一个学生
    void OffLineStudent(unsigned int Idx);

    //在不在上课
    bool m_bIsInClass;

    //服务端
    Server* m_pServer;

    //音频播放对象
    Audio* m_pVoiceCapture;

    //屏幕共享线程函数
    static DWORD WINAPI ScreenShareThread(LPVOID lpParameter);

    //屏幕共享线程句柄
    HANDLE m_hScreeShareThread;

    //是否正在屏幕共享
    bool m_bIsScreenSharing;

    //语音直播线程函数
    static DWORD WINAPI VoiceShareThread(LPVOID lpParameter);

    //语音直播线程句柄
    HANDLE m_hVoiceShareThread;

    //是否正在语音直播
    bool m_bIsVoiceSharing;

    //是否正在语音直播
    bool m_bIsVoiceSharing2;

    //是否正在发题 6-15
    bool m_bIsAskingQues;

    //对收到的消息进行执行的线程函数
    static DWORD WINAPI MsgReceiveThread(LPVOID lpParameter);

    //对收到的消息进行执行的线程句柄
    HANDLE m_hMsgReceiveThread;

    //保存用户名的数组*
    vector<string> m_Names;

    //保存密码的数组*
    vector<string> m_Passwords;

    //保存连接号的数组*
    vector<unsigned int> m_ConnectionIdx;

    //*标注星号，表示同一个学生的用户名、密码、连接号在三个数组中下标一致

    //收到学生发来的，自己先放着听
    Audio* m_pVoicePlayer;

    //所有接收到的注意力消息
    vector<string> m_AllFocusTimes;

    //所有收上来的选择题答案统计
    //未作答的在[0]里， 1 ~ 4 分别为ABCD的数量
    unsigned int m_allChoiceResult[5] = {0};

    //存下名字、答案和所用时间，总长度是3的倍数
    vector<string> m_QuestionData;

};

#endif // TCHR_PROCESS_H
