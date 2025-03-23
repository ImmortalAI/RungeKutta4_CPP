cmake_minimum_required(VERSION 3.10)
project(RungeKutta4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)  # Используем C++17 или новее
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем ExprTk
add_subdirectory(external/exprtk)

# Добавляем Qt (пример для Qt6)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

# Собираем наш проект
qt_add_executable(
    RungeKutta4 
    src/main.cpp 
    src/rk4.hpp
    src/mainwindow.cpp 
    src/mainwindow.ui
)

# Подключаем библиотеки
target_link_libraries(RungeKutta4 PRIVATE Qt6::Widgets exprtk)

set_target_properties(RungeKutta4 PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)