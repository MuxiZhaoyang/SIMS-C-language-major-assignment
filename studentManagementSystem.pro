# 项目构造文件（qmake）

QT += core gui
QT += sql    #加入数据库模块

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

RC_FILE = icon.rc

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    m_mainwindow.cpp \
    main.cpp \
    login.cpp \
    mainwindow.cpp \
    managerTerminal/src/Mchangepassword.cpp \
    managerTerminal/src/classmanagement.cpp \
    managerTerminal/src/coursemanagement.cpp \
    managerTerminal/src/scoremanagement.cpp \
    managerTerminal/src/selectionmanagement.cpp \
    managerTerminal/src/studentmanagement.cpp \
    managerTerminal/src/teachermanagement.cpp \
    smsdatabase.cpp \
    studentTerminal/src/changepassword.cpp \
    studentTerminal/src/courseinformation.cpp \
    studentTerminal/src/courseoverview.cpp \
    studentTerminal/src/courseselection.cpp \
    studentTerminal/src/scorequery.cpp \
    studentTerminal/src/userinfo.cpp

HEADERS += \
    login.h \
    m_mainwindow.h \
    mainwindow.h \
    managerTerminal/headfile/Mchangepassword.h \
    managerTerminal/headfile/classmanagement.h \
    managerTerminal/headfile/coursemanagement.h \
    managerTerminal/headfile/scoremanagement.h \
    managerTerminal/headfile/selectionmanagement.h \
    managerTerminal/headfile/studentmanagement.h \
    managerTerminal/headfile/teachermanagement.h \
    smsdatabase.h \
    studentTerminal/headfile/changepassword.h \
    studentTerminal/headfile/courseinformation.h \
    studentTerminal/headfile/courseoverview.h \
    studentTerminal/headfile/courseselection.h \
    studentTerminal/headfile/scorequery.h \
    studentTerminal/headfile/userinfo.h

FORMS += \
    login.ui \
    m_mainwindow.ui \
    mainwindow.ui \
    managerTerminal/ui/Mchangepassword.ui \
    managerTerminal/ui/classmanagement.ui \
    managerTerminal/ui/coursemanagement.ui \
    managerTerminal/ui/scoremanagement.ui \
    managerTerminal/ui/selectionmanagement.ui \
    managerTerminal/ui/studentmanagement.ui \
    managerTerminal/ui/teachermanagement.ui \
    studentTerminal/ui/changepassword.ui \
    studentTerminal/ui/courseinformation.ui \
    studentTerminal/ui/courseoverview.ui \
    studentTerminal/ui/courseselection.ui \
    studentTerminal/ui/scorequery.ui \
    studentTerminal/ui/userinfo.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rec.qrc

DISTFILES += \
    icon.rc
