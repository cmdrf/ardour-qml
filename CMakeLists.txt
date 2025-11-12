cmake_minimum_required(VERSION 3.16)

project(Ardour VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_QML_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR})

find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)

include(ardour.cmake)

add_subdirectory(qmlmodules/Ardour)


# Example Project
qt_add_executable(ExampleProject example/example.cpp)
qt_add_qml_module(ExampleProject
    URI ExampleProjectApp
    VERSION 1.0
    QML_FILES example/example.qml
)
target_link_libraries(ExampleProject PRIVATE
    Qt6::Quick
    Ardourplugin
    ardourlib
)
target_compile_definitions(ExampleProject PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
