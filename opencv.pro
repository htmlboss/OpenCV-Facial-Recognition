#-------------------------------------------------
#
# Project created by QtCreator 2015-03-25T18:15:50
#
#-------------------------------------------------

QT += core gui opengl multimedia network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14 console
TARGET = OpenCV-Facial-Recognition
TEMPLATE = app
RESOURCES += gui\style\Dark\style.qrc

RC_ICONS = gui\icon.ico

INCLUDEPATH += C:\opencv\msvc2015-x64\include

LIBS += -LC:\opencv\msvc2015-x64\lib \
 -lopencv_core310d -lopencv_imgcodecs310d -lopencv_imgproc310d -lopencv_objdetect310d -lopencv_video310d -lopencv_videoio310d -lopencv_videostab310d -lopencv_face310d -lopencv_ximgproc310d -lopencv_highgui310d

SOURCES += main.cpp \
    gui/mainwindow.cpp \
    camera.cpp \
    gui/displaywidget.cpp \
    faceDetector.cpp \
    gui/imageviewer.cpp \
    tcpserver.cpp

HEADERS += gui/mainwindow.h \
    camera.h \
    gui/displaywidget.h \
    faceDetector.h \
    gui/imageviewer.h \
    tcpserver.h
