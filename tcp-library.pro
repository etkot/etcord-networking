TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread

SOURCES += main.cpp \
    tcp/common.cpp \
    tcp/server.cpp \
    tcp/client.cpp

HEADERS += \
    tcp/common.h \
    tcp/server.h \
    tcp/client.h \
    tcp/safequeue.h \
    tcp/packet.h \
    tcp/sockets.h
