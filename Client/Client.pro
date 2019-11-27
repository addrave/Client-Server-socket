QT -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    mainwindow.cpp \
    tr.cpp


unix|win32: LIBS += -L$$PWD/../../../../Qt/Tools/mingw530_32/i686-w64-mingw32/lib/ -lws2_32

INCLUDEPATH += $$PWD/../../../../Qt/Tools/mingw530_32/i686-w64-mingw32/include
DEPENDPATH += $$PWD/../../../../Qt/Tools/mingw530_32/i686-w64-mingw32/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../../../Qt/Tools/mingw530_32/i686-w64-mingw32/lib/ws2_32.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../../../Qt/Tools/mingw530_32/i686-w64-mingw32/lib/libws2_32.a

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    tr.h
