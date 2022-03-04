QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addfriend.cpp \
    addgroup.cpp \
    chatlist.cpp \
    creategroup.cpp \
    group_chat.cpp \
    main.cpp \
    private_chat.cpp \
    recvthread.cpp \
    sendthread.cpp \
    widget.cpp

HEADERS += \
    addfriend.h \
    addgroup.h \
    chatlist.h \
    creategroup.h \
    group_chat.h \
    private_chat.h \
    recvthread.h \
    sendthread.h \
    widget.h

FORMS += \
    addfriend.ui \
    addgroup.ui \
    chatlist.ui \
    creategroup.ui \
    group_chat.ui \
    private_chat.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
