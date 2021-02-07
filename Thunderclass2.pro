QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Connection.cpp \
    Image.cpp \
    Pixel.cpp \
    Server.cpp \
    admin.cpp \
    admin_process.cpp \
    admin_win.cpp \
    audio.cpp \
    audioframe.cpp \
    client.cpp \
    login_process.cpp \
    main.cpp \
    login_win.cpp \
    message.cpp \
    mymessagebox.cpp \
    question.cpp \
    stu_process.cpp \
    student.cpp \
    student_ques_win3.cpp \
    student_win.cpp \
    student_win2.cpp \
    tchr_process.cpp \
    teacher.cpp \
    teacher_exit_win3.cpp \
    teacher_ques_win4.cpp \
    teacher_quesresult_win5.cpp \
    teacher_win.cpp \
    teacher_win2.cpp \
    user.cpp

HEADERS += \
    Connection.h \
    Image.h \
    MessageType.h \
    Pixel.h \
    Server.h \
    admin.h \
    admin_process.h \
    admin_win.h \
    audio.h \
    audioframe.h \
    client.h \
    login_process.h \
    login_win.h \
    message.h \
    mymessagebox.h \
    question.h \
    stu_process.h \
    student.h \
    student_ques_win3.h \
    student_win.h \
    student_win2.h \
    tchr_process.h \
    teacher.h \
    teacher_exit_win3.h \
    teacher_ques_win4.h \
    teacher_quesresult_win5.h \
    teacher_win.h \
    teacher_win2.h \
    user.h

FORMS += \
    admin_win.ui \
    login_win.ui \
    student_ques_win3.ui \
    student_win.ui \
    student_win2.ui \
    teacher_exit_win3.ui \
    teacher_ques_win4.ui \
    teacher_quesresult_win5.ui \
    teacher_win.ui \
    teacher_win2.ui

LIBS += -lws2_32 \
    -lgdi32 \
    -lgdiplus \
    -lwinmm \
    -lole32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
