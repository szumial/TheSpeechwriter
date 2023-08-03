QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include

HEADERS += \
    include/ContentManager.h \
    include/FileOperations.h \
    include/Sentence.h \
    include/SpeechwriterTableModel.h \
    include/MainWindow.h

SOURCES += \
    src/ContentManager.cpp \
    src/FileOperations.cpp \
    src/Sentence.cpp \
    src/SpeechwriterTableModel.cpp \
    src/main.cpp \
    src/MainWindow.cpp

FORMS += \
    forms/MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
