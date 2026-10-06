QT += widgets sql printsupport

CONFIG += c++17 utf8_source
msvc: QMAKE_CXXFLAGS += /utf-8

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    exports.cpp \
    hackathon.cpp \
    publications.cpp \
    affectation.cpp \
    connection.cpp \
    employe.cpp \
    gestionemployes.cpp \
    statswidget.cpp \
    tache.cpp \
    mainwindow.cpp

HEADERS += \
    competition.h \
    conflits.h \
    donut.h \
    exports.h \
    hackathon.h \
    publications.h \
    xlsx.h \
    affectation.h \
    connection.h \
    employe.h \
    gestionemployes.h \
    statswidget.h \
    tache.h \
    mainwindow.h

FORMS += \
    hackathon.ui \
    gestionemployes.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    style.qss \
    resources/style.qss \
    sql/script_oracle.sql

RESOURCES += \
    ressources.qrc \
    resources.qrc
