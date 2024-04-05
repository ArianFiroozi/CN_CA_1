QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Peer.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    Peer.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += /home/arian/Documents/libdatachannel/include
LIBS += -L/home/arian/Documents/libdatachannel/build -ldatachannel
LIBS += -L/home/arian/Qt/Tools/OpenSSLv3/src/util -lcrypto -lssl
