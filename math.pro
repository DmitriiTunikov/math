#-------------------------------------------------
#
# Project created by QtCreator 2019-06-15T22:08:36
#
#-------------------------------------------------

QT       -= gui

TARGET = math
TEMPLATE = lib

DEFINES += \
    MATH_LIBRARY \
    SHARED_EXPORTS

SOURCES += \
    Compact.cpp \
    Set.cpp \
    Vector.cpp

HEADERS += \
    error.h \
    SHARED_EXPORT.h \
    ICompact.h \
    ISet.h \
    IVector.h \
    Vector.h \
    Compact.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE32F9E55
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = math.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

QMAKE_CXXFLAGS += -std=gnu++0x






















