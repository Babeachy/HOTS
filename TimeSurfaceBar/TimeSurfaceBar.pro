#-------------------------------------------------
#
# Project created by QtCreator 2018-12-03T19:28:18
#
#-------------------------------------------------

QT       += core gui datavisualization

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimeSurfaceBar
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    timesurfacebar.cpp

HEADERS += \
        mainwindow.h \
    timesurfacebar.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/../../Research/SelfMadeLibraries/readMatSpikeData/x64/Debug/ -lreadMatSpikeData

INCLUDEPATH += $$PWD/../../Research/SelfMadeLibraries/readMatSpikeData/readMatSpikeData
DEPENDPATH += $$PWD/../../Research/SelfMadeLibraries/readMatSpikeData/readMatSpikeData

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../Research/SelfMadeLibraries/readMatSpikeData/x64/Debug/readMatSpikeData.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../../Research/SelfMadeLibraries/readMatSpikeData/x64/Debug/libreadMatSpikeData.a

win32: LIBS += -L$$PWD/'../../../Program Files (x86)/matlab_install/extern/lib/win64/mingw64/' -llibmat

INCLUDEPATH += $$PWD/'../../../Program Files (x86)/matlab_install/extern/include'
DEPENDPATH += $$PWD/'../../../Program Files (x86)/matlab_install/extern/include'

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/'../../../Program Files (x86)/matlab_install/extern/lib/win64/mingw64/libmat.lib'
else:win32-g++: PRE_TARGETDEPS += $$PWD/'../../../Program Files (x86)/matlab_install/extern/lib/win64/mingw64/liblibmat.a'

win32: LIBS += -L$$PWD/'../../../Program Files (x86)/matlab_install/extern/lib/win64/mingw64/' -llibmx

INCLUDEPATH += $$PWD/'../../../Program Files (x86)/matlab_install/extern/include'
DEPENDPATH += $$PWD/'../../../Program Files (x86)/matlab_install/extern/include'

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/'../../../Program Files (x86)/matlab_install/extern/lib/win64/mingw64/libmx.lib'
else:win32-g++: PRE_TARGETDEPS += $$PWD/'../../../Program Files (x86)/matlab_install/extern/lib/win64/mingw64/liblibmx.a'
