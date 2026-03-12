QT += core gui widgets charts

CONFIG += c++17

TEMPLATE = app
TARGET = IndustrialScadaQt

SOURCES += \
    src/main.cpp \
    src/datasimulator.cpp \
    src/usermanager.cpp \
    src/mainwindow.cpp \
    pages/dashboard.cpp \
    pages/monitoring.cpp \
    pages/controlpanel.cpp \
    pages/mimic.cpp \
    pages/filtrationdashboard.cpp \
    pages/alumclariflocculator.cpp \
    pages/alarms.cpp \
    pages/trends.cpp \
    pages/settings.cpp \
    widgets/tankwidget.cpp \
    widgets/statusindicator.cpp \
    widgets/sensorcard.cpp \
    widgets/eventlogwidget.cpp \
    widgets/processmimicwidget.cpp \
    widgets/valveindicator.cpp \
    widgets/processflowview.cpp \
    widgets/flowcomponents.cpp \
    widgets/alumclariflocculatorview.cpp

HEADERS += \
    src/datasimulator.h \
    src/usermanager.h \
    src/mainwindow.h \
    pages/dashboard.h \
    pages/monitoring.h \
    pages/controlpanel.h \
    pages/mimic.h \
    pages/filtrationdashboard.h \
    pages/alumclariflocculator.h \
    pages/alarms.h \
    pages/trends.h \
    pages/settings.h \
    widgets/tankwidget.h \
    widgets/statusindicator.h \
    widgets/sensorcard.h \
    widgets/eventlogwidget.h \
    widgets/processmimicwidget.h \
    widgets/valveindicator.h \
    widgets/processflowview.h \
    widgets/flowcomponents.h \
    widgets/alumclariflocculatorview.h
