QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += core widgets network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SEB/Event.cpp \
    SEB/EventModel.cpp \
    SEB/Material.cpp \
    SEB/RESTManager.cpp \
    main.cpp \
    MainWindow.cpp \
    widgets/AuthDialog.cpp \
    widgets/MaterialLine.cpp \
    widgets/ReturnDialog.cpp

HEADERS += \
    MainWindow.h \
    SEB/Event.h \
    SEB/EventModel.h \
    SEB/Material.h \
    SEB/RESTManager.h \
    widgets/AuthDialog.h \
    widgets/MaterialLine.h \
    widgets/ReturnDialog.h

FORMS += \
    MainWindow.ui \
    widgets/AuthDialog.ui \
    widgets/MaterialLine.ui \
    widgets/ReturnDialog.ui

TRANSLATIONS += \
    RobertPartner_fr_FR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

STATECHARTS +=
