QT       += core gui sql xml printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += "C:/Program Files/OpenSSL-Win64/include"
LIBS += "C:/Program Files/OpenSSL-Win64/lib/libcrypto.lib"
LIBS += "C:/Program Files/OpenSSL-Win64/lib/libssl.lib"
LIBS += "C:/Users/Kristijan/Documents/build-untitled2-Desktop_Qt_6_1_1_MinGW_64_bit-Debug/debug/Godisnji.dll"
LIBS += "C:/Users/Kristijan/Documents/build-untitled2-Desktop_Qt_6_1_1_MinGW_64_bit-Debug/debug/libParseClass.a"


SOURCES += \
    cipher.cpp \
    clienttcp.cpp \
    clientudp.cpp \
    httpdownload.cpp \
    main.cpp \
    loginwindow.cpp \
    mainwindow.cpp \
    table.cpp \
    threadworker.cpp

HEADERS += \
    Godisnji_global.h \
    cipher.h \
    clienttcp.h \
    clientudp.h \
    godisnji.h \
    httpdownload.h \
    loginwindow.h \
    mainwindow.h \
    parseclass.h \
    table.h \
    threadworker.h

FORMS += \
    clienttcp.ui \
    clientudp.ui \
    httpdownload.ui \
    loginwindow.ui \
    mainwindow.ui \
    table.ui

TRANSLATIONS += \
    untitled2_hr_HR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    languages.qrc

DISTFILES += \
    ../build-untitled2-Desktop_Qt_6_1_1_MinGW_64_bit-Debug/db/ntp_database.db

QMAKE_CXXFLAGS += -std=gnu++11
