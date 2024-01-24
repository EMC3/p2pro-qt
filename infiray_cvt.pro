QT       += core gui widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cmdline.cpp \
    colormap/colormap.cpp \
    colormap/hardcoded_colormaps.cpp \
    colormapmanager.cpp \
    displaywindow.cpp \
    historymonitor.cpp \
    logger/log.cpp \
    logger/loggerbackend.cpp \
    main.cpp \
    #mainwindow.cpp \
    marker.cpp \
    qcp/qcustomplot.cpp \
    settingswidget.cpp \
    thermalcamera.cpp \
    thermalplot.cpp \
    video4linuxwrappergeneric.cpp

HEADERS += \
    cmdline.h \
    colormap/colormap.h \
    colormap/hardcoded_colormaps.h \
    colormapmanager.h \
    displaywindow.h \
    historymonitor.h \
    logger/log.h \
    logger/loggerbackend.h \
    #mainwindow.h \
    marker.h \
    qcp/qcustomplot.h \
    settingswidget.h \
    thermalcamera.h \
    thermalplot.h \
    video4linuxwrappergeneric.h

FORMS += \
    displaywindow.ui \
    #mainwindow.ui \
    historymonitor.ui \
    settingswidget.ui


LIBS += -ltiff

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


#DEFINES+=O_BINARY=0
#LIBS += -lmlibs_full -lcrypto -lssl -lz -lyaml-cpp
#INCLUDEPATH += /mnt/d/QT/mlibs/mlibs
#LIBS += -L/mnt/d/QT/mlibs/build-mlibs-Desktop-Release/

RESOURCES += \
    darkstyle.qrc \
    res.qrc
