QT       += core

TARGET = lab1
TEMPLATE = app

QMAKE_MAC_SDK = macosx10.11

INCLUDEPATH += include

HEADERS += \
    include/ChildProcess.h \
    include/GrandChildProcess.h \
    include/MainProcess.h \
    include/Options.h \
    include/Pipe.h \
    include/Process.h \
    include/Utils.h

SOURCES += \
    src/main.cpp \
    \
    src/ChildProcess.cpp \
    src/GrandChildProcess.cpp \
    src/MainProcess.cpp \
    src/Options.cpp \
    src/Pipe.cpp \
    src/Process.cpp \
    src/Utils.cpp

OTHER_FILES += \
    MyMakefile \
    LICENSE \
    README \
    make_seed.py \
    data.txt
