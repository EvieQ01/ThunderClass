/*************************************************************************
【文件名】MessageType.h
【功能模块和目的】通用消息类型标识枚举类型的定义
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【版权信息】范静涛(fanjingtao@tsinghua.edu.cn)声明放弃对全部代码的任何版权诉求，任何使用者可做出任何修改、用于任何目的
【更改记录】
    2020-05-10 由范静涛做了命名规范化修改
    2020-05-15 由范静涛增加注释
    2020-06-09 由邱亦文添加 MSG_FOCUSTIME，注意力消息，用于“注意力收集”功能
                          MSG_CLASS_DISMISS，即将下课消息，用于“注意力收集”功能
    2020-06-12 由邱亦文添加 MSG_SHARE_YOUR_VOICE，用于“随机点名”
                          MSG_STOP_AUDIO，用于“随机点名”
    2020-06-14 由邱亦文添加 MSG_A_QUESTION，用于“发送题目”
                          MSG_COLLECT_ANS，用于“发送题目”
    2020-06-15 由邱亦文添加 MSG_MY_ANSWER，用于“发送题目”
    2020-06-20 由邱亦文添加 MSG_MULTI_QUESTION，MSG_MULTI_ANSWER,用于“多选题
*************************************************************************/
#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

enum MessageType {
    MSG_UNKOWN,   //未知消息类型
    MSG_ID_PSW,   //用户名密码消息类型
    MSG_EXIT,     //退出消息
    MSG_IDLE,     //空闲消息
    MSG_STRING,   //文本消息
    MSG_SCR_SIZE, //共享屏幕尺寸消息
    MSG_JPG,      //用JPG更新共享屏幕的消息
    MSG_SCR_COPY, //用覆盖法更新共享屏幕的消息
    MSG_SCR_XOR,  //用异或法更新共享屏幕的消息
    MSG_AUDIO,                        //音频消息
    MSG_FOCUSTIME,                    //学生端发出，注意力消息
    MSG_CLASS_DISMISS,                //教师端发出，即将下课
    MSG_SHARE_YOUR_VOICE,             //教师端发出，让某一学生端开麦
    MSG_STOP_AUDIO,                   //学生端或教师端发出，完成回答问题（两边处理函数有所不同）
    MSG_STOP_IMAGE,                   //教师端发出，停止共享
    MSG_A_QUESTION,                   //教师端发出，一道题
    MSG_MULTI_QUESTION,               //教师端发出，一道题，多选的！
    MSG_COLLECT_ANS,                  //教师端发出，完成全部答题
    MSG_A_ANSWER,                    //学生端发出，作答情况,单选
    MSG_MULTI_ANSWER,                //学生端发出，作答情况,多选
    MSG_MAXVALUE  //一定放在最后一个，防止增删类型导致代码错误
};
#endif // MESSAGETYPE_H
