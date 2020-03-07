#-------------------------------------------------
#
# Project created by QtCreator 2013-12-07T00:05:28
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HeavyBall
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    boule.cpp \
    plateau.cpp \
    conteneur.cpp \
    resultat.cpp \
    spriteviewer.cpp

HEADERS  += mainwindow.h \
    boule.h \
    plateau.h \
    conteneur.h \
    resultat.h \
    spriteviewer.h

FORMS    += mainwindow.ui \
    choix_niveau.ui

RC_FILE += ressources.rc

RESOURCES += \
    Resources.qrc
