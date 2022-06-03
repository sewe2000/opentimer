QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    lastsolvesdashboard.cpp \
    main.cpp \
    mainwindow.cpp \
    regularcubemap.cpp \
    scramblegenerator.cpp \
    timer.cpp

HEADERS += \
    lastsolvesdashboard.h \
    mainwindow.h \
    regularcubemap.h \
    scramblegenerator.h \
    timer.hpp \
    types.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

unix:!macx: LIBS += -lusb-1.0