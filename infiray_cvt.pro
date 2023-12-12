QT       += core gui widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cmdline.cpp \
    colormap/colormap.cpp \
    colormap/hardcoded_colormaps.cpp \
    logger/log.cpp \
    logger/loggerbackend.cpp \
    main.cpp \
    mainwindow.cpp \
    qcp/qcustomplot.cpp \
    thermalcamera.cpp \
    video4linuxwrappergeneric.cpp

HEADERS += \
    cmdline.h \
    colormap/colormap.h \
    colormap/hardcoded_colormaps.h \
    logger/log.h \
    logger/loggerbackend.h \
    mainwindow.h \
    qcp/qcustomplot.h \
    thermalcamera.h \
    video4linuxwrappergeneric.h

FORMS += \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


#DEFINES+=O_BINARY=0
#LIBS += -lmlibs_full -lcrypto -lssl -lz -lyaml-cpp
#INCLUDEPATH += /mnt/d/QT/mlibs/mlibs
#LIBS += -L/mnt/d/QT/mlibs/build-mlibs-Desktop-Release/

RESOURCES += \
    res.qrc
