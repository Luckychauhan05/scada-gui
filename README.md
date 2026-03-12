# Industrial SCADA GUI (Qt Widgets)

Professional, modular SCADA-style interface for water treatment monitoring and control.

## Features

- Dark industrial operator UI
- Left navigation + top status bar + stacked center pages
- Dashboard with tank widgets, pump indicators, sensor cards, health/alarm summary
- Device monitoring table with live simulated updates
- Control panel with command confirmations and emergency stop
- Alarm manager with severity highlighting + acknowledgment
- Historical trends using Qt Charts (`QChartView`)
- Settings page for communication, thresholds, users, and DB config
- Real-time simulation via `QTimer`

## Build (CMake)

Open the folder in Qt Creator and select [CMakeLists.txt](CMakeLists.txt).

Or in terminal:

- Configure: `cmake -S . -B build`
- Build: `cmake --build build`
- Run: `./build/IndustrialScadaQt`

## Build (qmake)

Open [IndustrialScadaQt.pro](IndustrialScadaQt.pro) in Qt Creator and build.

## Project Layout

- [src/main.cpp](src/main.cpp)
- [src/mainwindow.h](src/mainwindow.h)
- [src/mainwindow.cpp](src/mainwindow.cpp)
- [pages](pages)
- [widgets](widgets)
