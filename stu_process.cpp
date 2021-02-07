/*************************************************************************
【文件名】Stu_Process.cpp
【功能模块和目的】学生业务流程类定义
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
    2020-06-20 由邱亦文添加发送多选题答案（多态SendChoiceAndTime）
*************************************************************************/
#include "stu_process.h"
#include <windows.h>
#include <QDebug>

/*************************************************************************
【函数名称】Stu_Process::Stu_Process
【函数功能】构造函数
【参数】入口参数，父对象指针，默认nullptr
【返回值】构造函数不可有返回值
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 由范静涛删除界面空间指针参数，用信号实现界面更新
    2020-05-19 由范静涛修正了未初始化消息接收处理线程句柄为 nullptr的错误逻辑
    2020-06-11 由邱亦文更新对发送语音状态的初始化
    2020-06-20 由邱亦文添加发送多选题答案（多态SendChoiceAndTime）
*************************************************************************/
Stu_Process::Stu_Process(QObject* Parent): QObject(Parent)
             , IsInClass(m_bIsInClass), IsVoiceSending(m_bIsVoiceSending)
{
    //准备一个用于远程登录的消息，包含自己的用户名和密码
    //GetLoginedUser获取的是本地登录的账户指针
    m_RemoteLoginMsg = User::GetLoginedUser()->ToMessage();
    //用私有成员保存界面控件指针
    //初始化内部状态
    m_bIsInClass = false;
    m_bIsVoiceSending = false;
    //新建录音对象
    m_pVoiceCapture = new Audio();
    //新建socket客户端
    m_pClient = new Client();
    //新建音频播放对象
    m_pVoicePlayer = new Audio();
    m_pVoicePlayer->StopRec();
    //初始化消息接收处理线程句柄为空
    m_hMsgReceiveThread = nullptr;
    connect(this, SIGNAL(VoiceSending()), this, SLOT(StartSendingVoice()));
    qDebug("SP: Created");
}

/*************************************************************************
【函数名称】Stu_Process::~Stu_Process
【函数功能】析构函数
【参数】无
【返回值】析构函数不可有返回值
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
*************************************************************************/
Stu_Process::~Stu_Process() {
    //退出课堂
    ExitClass();
    //释放socket客户端和音频对象
    delete m_pClient;
    m_pClient = nullptr;
    delete m_pVoicePlayer;
    m_pVoicePlayer = nullptr;
    delete m_pVoiceCapture;
    m_pVoiceCapture = nullptr;
    qDebug("SP: Destroyed");
}

/*************************************************************************
【函数名称】Stu_Process::EnterClass
【函数功能】进入课堂
【参数】入口参数，教师端IP地址
【返回值】true表示当前连接状态（是否在课堂里）
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 由范静涛修改未用信号实现界面更新
*************************************************************************/
bool Stu_Process::EnterClass(const QString& IP) {
    //如果已经进入课堂，则不执行任何操作
    if (m_bIsInClass) {
        return m_bIsInClass;
    }

    //客户端连接到服务器
    string IpStr = IP.toStdString();
    m_pClient->ConnectTo(IpStr, 3004);

    //开始计时
    qDebug() << "SP: trying to connect to server @ " << QString::fromStdString(IpStr);

    DWORD BeginTime = GetTickCount();
    DWORD EndTime = BeginTime;
    do {
        //更新当前时间
        EndTime = GetTickCount();
    //30秒内未连接，则继续循环
    }while(EndTime - BeginTime <= 30000 && !m_pClient->IsConnected);

    //连接失败，返回
    if (!m_pClient->IsConnected) {
        qDebug() << "SP: connect fail after " << EndTime - BeginTime << " ms";
        //发出“退出课堂信号”
        emit ClassExited();
        return false;
    }

    //更新为以进入课堂状态
    m_bIsInClass = true;
    //发出“进入课堂信号”
    qDebug() << "SP: connected after " << EndTime - BeginTime << " ms";
    emit ClassEntered();
    //创建消息接收和处理线程
    m_hMsgReceiveThread = CreateThread(nullptr, 0, MsgReceiveThread, (LPVOID)this, 0, nullptr);
    if (m_hMsgReceiveThread == nullptr) {
        qDebug() << "SP: failed to create a MessageRecv&Process thread!";
        throw(runtime_error("Failed to create a MessageRecv&Process thread!"));
    }
    //第一时间向教师端发送登录用的用户名密码消息
    qDebug() << "SP: send ID & Password";
    m_pClient->Send(m_RemoteLoginMsg);
    //开始播放声音(如果从教师端收到了)
    m_pVoicePlayer->StartPlay();

    return m_bIsInClass;
}

/*************************************************************************
【函数名称】Stu_Process::ExitClass
【函数功能】退出课堂
【参数】无
【返回值】true表示当前连接状态（是否在课堂里）
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 由范静涛修改未用信号实现界面更新
    2020-06-11 由邱亦文更新对音频对象的清理
*************************************************************************/
bool Stu_Process::ExitClass() {
    //如果未在课堂中，检查是否消息接收处理线程自主退出了
    if (!m_bIsInClass) {
        if(m_hMsgReceiveThread != nullptr) {
            qDebug() << "SP: clean MsgRecv&Process thread handle";
            CloseHandle(m_hMsgReceiveThread);
            m_hMsgReceiveThread = nullptr;
        }
        return m_bIsInClass;
    }
    //停止播放音频
    m_pVoicePlayer->StopPlay();
    //清空待播放音频队列
    m_pVoicePlayer->Clear();
    //停止录音
    m_pVoiceCapture->StopRec();
    //清空录音队列
    m_pVoiceCapture->Clear();
    //更新为未上课状态
    m_bIsInClass = false;
    //更新为停止说话
    m_bIsVoiceSending = false;
    //断开与服务器连接
    m_pClient->DisConnect();
    //等待消息接收和处理线程结束
    qDebug() << "SP: wait MsgRecv & Process thread end";
    WaitForSingleObject(m_hMsgReceiveThread, 2000);
    //清理线程句柄
    qDebug() << "SP: clean MsgRecv & Process thread handle";
    CloseHandle(m_hMsgReceiveThread);
    m_hMsgReceiveThread = nullptr;

    //返回当前是否在课堂中
    return m_bIsInClass;
}

/*************************************************************************
【函数名称】Stu_Process::Send
【函数功能】向教师端发送文字消息
【参数】入口参数，QString字符串
【返回值】true表示当前连接状态（是否在课堂里）
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
*************************************************************************/
bool Stu_Process::Send(const QString& Msg) {
    //不在课堂
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    //未连接教师端
    if (!m_pClient->IsConnected) {
        return m_pClient->IsConnected;
    }
    //装换格式并发送
    qDebug() << "SP: send Message: " << Msg;
    string MsgStr = Msg.toStdString();
    return m_pClient->Send(Message::FromString(MsgStr));
}


/*************************************************************************
【函数名称】Stu_Process::SendFocusData
【函数功能】向教师端发送注意力
【参数】入口参数，QTime1总时长，QTime2专注时长
【返回值】true表示当前连接状态（是否在课堂里）
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-10
【更改记录】
*************************************************************************/
bool Stu_Process::SendFocusData(const QTime& t1,const QTime& t2) {
    //不在课堂
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    //未连接教师端
    if (!m_pClient->IsConnected) {
        return m_pClient->IsConnected;
    }
    //装换格式并发送
    qDebug() << "SP: send Focus Data Message: " << t1.toString() + " / " + t2.toString();
    m_FocusTimeMsg = Message::FromTimeData(t1, t2);
    return m_pClient->Send(m_FocusTimeMsg);
}




/*************************************************************************
【函数名称】Stu_Process::SendChoiceAndTime
【函数功能】向教师端发送作答答案
【参数】入口参数，int选择，QTime所用时长
【返回值】true表示当前连接状态（是否在课堂里）
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-16
【更改记录】
*************************************************************************/
bool Stu_Process::SendChoiceAndTime(const int result, const QTime& time) {
    //不在课堂
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    //未连接教师端
    if (!m_pClient->IsConnected) {
        return m_pClient->IsConnected;
    }
    //装换格式并发送
    qDebug() << "SP: send my choice:  " << result;
    qDebug() << " (using time "<<time.toString() << " )";
    Message msg = Message::FromMyAnswer(result, time);
    return m_pClient->Send(msg);
}


/*************************************************************************
【函数名称】Stu_Process::SendChoiceAndTime
【函数功能】向教师端发送注意力
【参数】入口参数，int选择，QTime所用时长
【返回值】true表示当前连接状态（是否在课堂里）
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-20
【更改记录】
*************************************************************************/
bool Stu_Process::SendChoiceAndTime(const int result[], const QTime& time) {
    //不在课堂
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    //未连接教师端
    if (!m_pClient->IsConnected) {
        return m_pClient->IsConnected;
    }
    //装换格式并发送
    qDebug() << "SP: send my choice:  " << result;
    qDebug() << " (using time "<<time.toString() << " )";
    Message msg = Message::FromMyAnswer(result, time);
    return m_pClient->Send(msg);
}





/*************************************************************************
【函数名称】Stu_Process::StartSendingVoice
【函数功能】开始向教师端发送语音
【参数】无
【返回值】true表示当前连接状态（是否在课堂里）
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-11
【更改记录】
*************************************************************************/
bool Stu_Process::StartSendingVoice() {
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    if (m_bIsVoiceSending) {
        return m_bIsVoiceSending;
    }
    m_pVoiceCapture->StartRec();
    m_bIsVoiceSending = true;
    qDebug() << "SP: start voice share(to TP)";
    SendingVoice();
    return m_bIsVoiceSending;
}

/*************************************************************************
【函数名称】Stu_Process::SendingVoice
【函数功能】向教师端发送一个语音
【参数】无
【返回值】无
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-11
【更改记录】
    2020-06-12 由邱亦文修改为在 MsgRecvThread中进行循环
*************************************************************************/
void Stu_Process::SendingVoice() {
    AudioFrame TempFrame;
    //取出一个录制好的音频帧
    if (m_pVoiceCapture->GetAndRemoveFirstRecorded(TempFrame)) {
        qDebug() << "SP: remove an audio frame";
        //如果平均未发消息不足10，就抓声音，转换成音频消息发出去
        if (m_pClient->ToSendCount() <= 10) {
            m_pClient->Send(Message::FromAudio(TempFrame));
            qDebug() << "SP: send an audio frame";
        }
    }
}



/*************************************************************************
【函数名称】Stu_Process::StopSendingVoice
【函数功能】停止语音发送
【参数】无
【返回值】false为当前停止语音发送
【开发者及日期】邱亦文(qyw19@mails.tsinghua.edu.cn) 2020-06-11
【更改记录】
    2020-06-12 由邱亦文修改为在 MsgRecvThread中进行循环

*************************************************************************/
//注意：返回值仍然是是否在发出声音
bool Stu_Process::StopSendingVoice() {
    if (!m_bIsVoiceSending) {
        return m_bIsVoiceSending;
    }
    if (!m_bIsInClass) {
        return m_bIsInClass;
    }
    m_pVoiceCapture->StopRec();
    m_bIsVoiceSending = false;
    AudioFrame TempFrame;
    while(m_pVoiceCapture->RecordFramesCount() > 0) {
        m_pVoiceCapture->GetAndRemoveFirstRecorded(TempFrame);
    }
    qDebug() << "SP: clean voice share queue";
    qDebug() << "SP: stop voice share";
    m_pClient->Send(Message::MakeVoiceStopMsg());
    return m_bIsVoiceSending;
}






/*************************************************************************
【函数名称】Stu_Process::MsgRecieveThread
【函数功能】接收并处理消息的线程函数
【参数】入口参数，表示线程所属的Stu_Process对象指针
【返回值】无意义
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-12
【更改记录】
    2020-05-19 由范静涛修改未用信号实现界面更新
    2020-05-19 由范静涛修正了jpg空间未释放的错误逻辑
    2020-06-10 由邱亦文增添对即将下课 MSG的处理
    2020-06-12 由邱亦文增添对被点名开麦 MSG的处理
    2020-06-13 由邱亦文增添对停止语音 MSG的处理; 修改为在此线程函数中向教师发送语音
    2020-06-15 由邱亦文增添对教师发题 MSG的处理
    2020-06-16 由邱亦文增添对教师收题 MSG的处理
               由邱亦文增添对教师停止共享屏幕 MSG的处理
*************************************************************************/
DWORD WINAPI Stu_Process::MsgReceiveThread(LPVOID lpParameter) {
    //因为实参是Stu_Process*，所以可以强制类型装换
    Stu_Process* pProcess = (Stu_Process*)lpParameter;
    //用于接收的临时消息存储对象
    Message TempMessage;
    //接收到的消息的类型
    MessageType Type;
    //在课堂且通信中，始终执行循环
    char* pJpgData = nullptr;
    while (pProcess->IsInClass && pProcess->m_pClient->IsConnected) {
        if (pProcess->IsVoiceSending) {
            pProcess->SendingVoice();
        }
        //有待处理消息，则放入TempMessage里
        if (pProcess->m_pClient->GetAndRemoveFirstRecvMsg(TempMessage)) {
            //获得消息类型
            Type = TempMessage.Type();
            //如果是文本信息消息，转换成为本对象，发出信号要求界面更新
            if (Type == MSG_STRING) {
                qDebug() << "SP: recieved chat message: " << QString::fromStdString(Message::ToString(TempMessage));
                emit pProcess->RecvChat(QString::fromStdString(Message::ToString(TempMessage)));
            }
            //如果是音频帧消息，放入待播放音频队列中
            else if (Type == MSG_AUDIO) {
              //  qDebug() << "SP: recieved audio frame message " << TempMessage.Size << " bytes";
                pProcess->m_pVoicePlayer->AppendFrameToPlay(Message::ToAudioFrame(TempMessage));
            }
            //如果是Jpg图像消息，界面后刷到界面上
            else if (Type == MSG_JPG) {
                pJpgData = nullptr;
                unsigned int JpgSize = 0;
                Message::ToJpgData(TempMessage, &pJpgData, &JpgSize);
                QImage qImg;
                qImg.loadFromData((unsigned char*)pJpgData, JpgSize, "jpg");
              //  qDebug() << "SP: recieved jpg message " << JpgSize << " bytes";
                //2020-05-19 修正了逻辑，之前没有释放开辟的空间
                delete [] pJpgData;
                pJpgData = nullptr;
                //发出收到图像信号，要求界面更新
                emit pProcess->RecvScreen(qImg);
            }
            //如果是退出消息，跳出消息接收处理循环，相当退出课堂
            else if (Type == MSG_EXIT) {
                pProcess->m_bIsInClass = false;
                qDebug() << "SP: disconnected socket";
                //发出退出课堂信号
                emit pProcess->ClassExited();
                break;
            }
            else if(Type == MSG_CLASS_DISMISS) {
                qDebug() << "SP: sending focus time";
                //发出准备发送注意力信号
                emit pProcess->ShouldSendFocusData();
            }
            else if(Type == MSG_SHARE_YOUR_VOICE) {
                qDebug() << "SP: ready to voice your answer";
                //发出准备开麦信号
                emit pProcess->VoiceSending();
               // pProcess->StartSendingVoice();
            }
            else if (Type == MSG_STOP_AUDIO) {
                qDebug() << "SP: teacher ask you to finish answering";
                pProcess->StopSendingVoice();
                //发出回答完问题的信号
                emit pProcess->StopVoiceSending();
            }
            else if (Type == MSG_A_QUESTION || Type == MSG_MULTI_QUESTION) {
                qDebug() << "SP: received a question";
                //发出收到一个问题的信号 6-15
                Question tempQues = Message::ToAQuestion(TempMessage);
                QString qProb = QString::fromStdString(tempQues.GetProb());
                QString qA = QString::fromStdString(tempQues.GetChoiceA());
                QString qB = QString::fromStdString(tempQues.GetChoiceB());
                QString qC = QString::fromStdString(tempQues.GetChoiceC());
                QString qD = QString::fromStdString(tempQues.GetChoiceD());
                if (Type == MSG_A_QUESTION) {
                    emit pProcess->RecvAQues(qProb, qA, qB, qC, qD);
                }
                else {
                    emit pProcess->RecvMultiQues(qProb, qA, qB, qC, qD);
                }
            }
            else if (Type == MSG_COLLECT_ANS) {
                emit pProcess->RecvFinishChoosing();
            }
            else if (Type == MSG_STOP_IMAGE) {
                emit pProcess->RecvScreenStop();
            }
        }
    }
    return 0;
}
