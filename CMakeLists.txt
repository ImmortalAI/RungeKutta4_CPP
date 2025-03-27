cmake_minimum_required(VERSION 3.10)
project(RungeKutta4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)  # Using C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/exprtk)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets PrintSupport)
qt_standard_project_setup()

qt_add_executable(
    RungeKutta4 
    external/qcustomplot/qcustomplot.cpp
    src/main.cpp 
    src/rk4.hpp
    src/mainwindow.cpp 
    src/mainwindow.ui
    src/graphwindow.cpp
    src/graphwindow.ui
)

target_include_directories(RungeKutta4 PRIVATE external/qcustomplot)

target_link_libraries(RungeKutta4 PRIVATE Qt6::Widgets Qt6::Core Qt6::PrintSupport exprtk)

set_target_properties(RungeKutta4 PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)