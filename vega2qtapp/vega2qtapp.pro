QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = ../bin
TARGET = vega2qtapp
TEMPLATE = app

INCLUDEPATH += ../vega2lib ../iANPR1.3/include

LIBS += -L"$$PWD/../bin"
LIBS += -L/usr/local/lib
LIBS += -lvega2lib
LIBS += -lboost_system -lboost_thread -lboost_filesystem
equals(QMAKE_HOST.arch, x86_64) {
	LIBS += -L"$$PWD/../iANPR1.3/x64"
}
equals(QMAKE_HOST.arch, i686) {
	LIBS += -L"$$PWD/../iANPR1.3/x86"
}

LIBS += -lianpr1.3
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_gpu -lopencv_video

SOURCES += main.cpp \
        anprapp.cpp \
        imagewidget.cpp

HEADERS += anprapp.h \
        imagewidget.h

FORMS += anprapp.ui
