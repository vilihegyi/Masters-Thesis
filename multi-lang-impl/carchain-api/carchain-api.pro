HEADERS += \
    src/Client.h \
    src/Const.h \
    src/Daemon.h \
    src/Request.h \
    src/RequestFactory.h \
    src/Singleton.h \
    src/SHA256.h \
    src/Car.h \
    src/Blockchain.h \
    src/Block.h

SOURCES += \
    src/main.cpp \
    src/Client.cpp \
    src/Daemon.cpp \
    src/Request.cpp \
    src/RequestFactory.cpp \
    src/SHA256.cpp \
    src/Car.cpp \
    src/Blockchain.cpp \
    src/Block.cpp

CONFIG = qt debug c++11
QT = core sql websockets

*-g++* {
    QMAKE_CXXFLAGS += "-Wall -Wextra -pedantic"
}

isEmpty(PREFIX) {
    PREFIX=$$(prefix)
    isEmpty(PREFIX) {
        warning("Neither qmake variable 'PREFIX' nor environment variable 'prefix' was defined.")
        PREFIX=/usr
    }
}
target.path = $$PREFIX/sbin
INSTALLS += target
