QT -= gui

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    sevrovlibrary.cpp \
    sevrovxboxcontroller.cpp

HEADERS += \
    sevrovlibrary.h \
    sevrovxboxcontroller.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

LIBS += -L D:/SDL2-2.28.2-mingw/x86_64-w64-mingw32/lib -lSDL2
INCLUDEPATH += D:/SDL2-2.28.2-mingw/x86_64-w64-mingw32/include
