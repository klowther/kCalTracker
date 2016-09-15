#-------------------------------------------------
#
# Project created by QtCreator 2016-08-02T18:57:15
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kCalTracker
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    AddFoodFormDialog.cpp \
    FoodData.cpp \
    DatabaseManager.cpp

HEADERS  += MainWindow.h \
    AddFoodFormDialog.h \
    FoodData.h \
    DatabaseManager.h

FORMS    += MainWindow.ui \
    AddFoodFormDialog.ui
