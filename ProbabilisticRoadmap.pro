TEMPLATE = app
QT += core
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += main.cpp \
    sourcedata.cpp \
    polygon.cpp \
    configurationspace.cpp \
    probabilisticroadmapstrategy.cpp \
    motion.cpp \
    graph.cpp

HEADERS += \
    sourcedata.h \
    polygon.h \
    configurationspace.h \
    probabilisticroadmapstrategy.h \
    motion.h \
    graph.h
