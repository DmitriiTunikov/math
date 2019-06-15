#-------------------------------------------------
#
# Project created by QtCreator 2019-06-15T22:08:36
#
#-------------------------------------------------

QT       -= gui

TARGET = math
TEMPLATE = lib

DEFINES += MATH_LIBRARY

SOURCES += \
    Compact/Compact.cpp \
    Set/Set.cpp \
    Vector/Vector.cpp

HEADERS += \
    ISolver.h \
    error.h \
    SHARED_EXPORT.h \
    Compact/ICompact.h \
    Set/ISet.h \
    Vector/IVector.h \
    Vector/Vector.h

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




















