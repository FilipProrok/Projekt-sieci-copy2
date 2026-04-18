QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Backend/Generator.cpp \
    Backend/ModelARX.cpp \
    Backend/ProstyUAR.cpp \
    Backend/RegulatorPID.cpp \
    Backend/Services.cpp \
    Backend/Testy_UAR.cpp \
    Backend/filehandler.cpp \
    config.cpp \
    dialog.cpp \
    klienttcp.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    serwertcp.cpp \
    wykres.cpp

HEADERS += \
    Backend/Generator.h \
    Backend/ModelARX.h \
    Backend/ProstyUAR.h \
    Backend/RegulatorPID.h \
    Backend/Services.h \
    Backend/filehandler.h \
    config.h \
    dialog.h \
    klienttcp.h \
    mainwindow.h \
    qcustomplot.h \
    serwertcp.h \
    wykres.h

FORMS += \
    dialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
QT += printsupport
QMAKE_CXXFLAGS += -Wa,-mbig-obj #WAŻNE, BEZ TEGO QT 6_6_0 NIE ODPALI QCUSTOMPLOT
