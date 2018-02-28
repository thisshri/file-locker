#-------------------------------------------------
#
# Project created by QtCreator 2016-10-02T14:04:29
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Locker
TEMPLATE = app


SOURCES +=\
        loginwindow.cpp \
    userregistration.cpp \
    vault.cpp \
    db.cpp

HEADERS  += loginwindow.h \
    userregistration.h \
    vault.h \
    db.h \
    username.h \
    pathandhandle.h

FORMS    += loginwindow.ui \
    userregistration.ui \
    vault.ui

DISTFILES += \
    LockerDb.sqlite
