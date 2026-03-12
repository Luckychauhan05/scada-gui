cmake_minimum_required(VERSION 3.16)
project(IndustrialScadaQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

add_executable(IndustrialScadaQt
    src/main.cpp
    src/datasimulator.cpp
    src/datasimulator.h
    src/usermanager.cpp
    src/usermanager.h
    src/mainwindow.cpp
    src/mainwindow.h

    pages/dashboard.cpp
    pages/dashboard.h
    pages/monitoring.cpp
    pages/monitoring.h
    pages/controlpanel.cpp
    pages/controlpanel.h
    pages/mimic.cpp
    pages/mimic.h
    pages/filtrationdashboard.cpp
    pages/filtrationdashboard.h
    pages/alumclariflocculator.cpp
    pages/alumclariflocculator.h
    pages/alarms.cpp
    pages/alarms.h
    pages/trends.cpp
    pages/trends.h
    pages/settings.cpp
    pages/settings.h

    widgets/tankwidget.cpp
    widgets/tankwidget.h
    widgets/statusindicator.cpp
    widgets/statusindicator.h
    widgets/sensorcard.cpp
    widgets/sensorcard.h
    widgets/eventlogwidget.cpp
    widgets/eventlogwidget.h
    widgets/processmimicwidget.cpp
    widgets/processmimicwidget.h
    widgets/valveindicator.cpp
    widgets/valveindicator.h
    widgets/blowercontrolpanel.cpp
    widgets/blowercontrolpanel.h
    widgets/processflowview.cpp
    widgets/processflowview.h
    widgets/flowcomponents.cpp
    widgets/flowcomponents.h
    widgets/alumclariflocculatorview.cpp
    widgets/alumclariflocculatorview.h
    widgets/pumpcontrolfaceplate.cpp
    widgets/pumpcontrolfaceplate.h
)

target_include_directories(IndustrialScadaQt PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(IndustrialScadaQt PRIVATE Qt6::Widgets Qt6::Charts)
