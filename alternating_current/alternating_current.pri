QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SOURCES += \
    $$PWD/../../tinyxml/tinystr.cpp \
    $$PWD/../../tinyxml/tinyxml.cpp \
    $$PWD/../../tinyxml/tinyxmlerror.cpp \
    $$PWD/../../tinyxml/tinyxmlparser.cpp \
    $$PWD/branch.cpp \
    $$PWD/functions.cpp \
    $$PWD/graph.cpp

HEADERS += \
    $$PWD/../../tinyxml/tinystr.h \
    $$PWD/../../tinyxml/tinyxml.h \
    $$PWD/branch.h \
    $$PWD/element.h \
    $$PWD/enum.h \
    $$PWD/functions.h \
    $$PWD/graph.h

INCLUDEPATH = \
    $$PWD/.
