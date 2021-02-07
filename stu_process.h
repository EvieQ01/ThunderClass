/*************************************************************************
【文件名】Stu_Process.h
【功能模块和目的】学生业务流程类声明
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【版权信息】开发者范静涛(fanjingtao@tsinghua.edu.cn)声明放弃任何版权诉求，任何使用者可做出任何修改、用于任何目的
【更改记录】
    2020-05-19 由范静涛修改为从QObject继承，用信号实现界面更新
    2020-06-10 由邱亦文增添“发送注意力时长”功能
    2020-06-12 由邱亦文增添“（被）随机点名开麦”功能，能够发送语音
    2020-06-13 由邱亦文完善“（被）随机点名开麦”功能，能够停止语音，或者被教师要求停止语音
    2020-06-15 由邱亦文增添“接收题目”功能
    2020-06-16 由邱亦文增添“发送作答情况”功能，可以自行结束答题，或教师一键结束答题
    2020-06-16 由邱亦文增添注释

*************************************************************************/

#ifndef STU_PROCESS_H
#define STU_PROCESS_H


#include <QObject>
#include <QWidget>
#include <QString>
#include <QComboBox>
#include <QScrollArea>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <string>
#include "client.h"
#include "message.h"
#include "audio.h"
#include "user.h"

using namespace std;

/*************************************************************************
【类名】StudentProcess
【功能】学生上课业务流程类
【接口说明】
    构造函数StudentProcess
    析构函数~StudentProcess
    函数 EnterClass进入指定IP的课堂
    函数 ExitClass退出课堂
    函数 Send向教师发送文本消息
    函数 SendFocusData发送注意力消息
    函数 SendChoiceAndTime发送作答结果和所用时间
    函数 SendingVoice一直转播
    函数 StopSendingVoice停止转播

    常引用数据成员 IsVoiceSending 是否在转播语音
    常引用数据成员 IsInClass表示是否在上课

【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 由范静涛修改为从QObject继承，用信号实现界面更新
    2020-06-10 由邱亦文增添“发送注意力时长”功能
    2020-06-12 由邱亦文增添“（被）随机点名开麦”功能，能够发送语音
    2020-06-13 由邱亦文完善“（被）随机点名开麦”功能，能够停止语音，或者被教师要求停止语音
    2020-06-15 由邱亦文增添“接收题目”功能
    2020-06-16 由邱亦文增添“发送作答情况”功能，可以自行结束答题，或教师一键结束答题
    2020-06-16 由邱亦文增添注释
    2020-06-20 由邱亦文添加发送多选题答案（多态SendChoiceAndTime）
*************************************************************************/
class Stu_Process : public QObject{
    Q_OBJECT
public:
    //构造函数
    explicit Stu_Process(QObject* Parent = nullptr);

    //析构函数
    ~Stu_Process();

    //禁用拷贝构造函数
    Stu_Process(const Stu_Process&) = delete;

    //禁用赋值运算符
    Stu_Process& operator=(const Stu_Process&) = delete;

    //进入课堂
    bool EnterClass(const QString& IP);

    //退出课堂
    bool ExitClass();

    //发送文本消息给教师
    bool Send(const QString& Msg);

    //是否在上课
    const bool& IsInClass;

    //发送注意力消息
    bool SendFocusData(const QTime& t1, const QTime& t2);

    //发送作答结果和所用时间(单选)
    bool SendChoiceAndTime(const int result, const QTime& time);

    //发送作答结果和所用时间(多选)
    bool SendChoiceAndTime(const int result[], const QTime& time);

    //是否在转播语音
    const bool& IsVoiceSending;

    //一直转播
    void SendingVoice();

    //停止转播
    bool StopSendingVoice();

signals:
    //进入课堂信号
    void ClassEntered();
    //退出课堂信号
    void ClassExited();
    //收到文字消息信号
    void RecvChat(QString Msg);
    //收到屏幕共享消息信号
    void RecvScreen(QImage Img);

    //应该发送注意力消息
    void ShouldSendFocusData();

    //开麦回答问题
    void VoiceSending();

    //被教师关了麦克
    void StopVoiceSending();

    //6-15由邱亦文添加，收到一单选道题
    void RecvAQues(QString prob, QString choiceA, QString choiceB, QString choiceC, QString choiceD);

    //6-20由邱亦文添加，收到一多选道题
    void RecvMultiQues(QString prob, QString choiceA, QString choiceB, QString choiceC, QString choiceD);

    //6-16由邱亦文添加，作答完毕
    void RecvFinishChoosing();

    //6-16由邱亦文添加，教师停止共享
    void RecvScreenStop();
private:
    //是否在上课
    bool m_bIsInClass;

    //(录音)音频播放对象
    Audio* m_pVoiceCapture;
    //是否在转播语音
    bool m_bIsVoiceSending;

    //Socket客户端对象指针
    Client* m_pClient;

    //音频播放对象指针
    Audio* m_pVoicePlayer;

    //接收消息并处理消息的线程函数
    static DWORD WINAPI MsgReceiveThread(LPVOID lpParameter);

    //对收到的消息进行执行的线程句柄
    HANDLE m_hMsgReceiveThread;
    Message m_RemoteLoginMsg;

    //注意力消息
    Message m_FocusTimeMsg;

private slots:
    //开始转播语音
    bool StartSendingVoice();
};

#endif // STUDENTPROCESS_H
