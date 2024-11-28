cmake_minimum_required(VERSION 3.16)
project(BinarySearchTreeVisualization VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Add resources
set(PROJECT_RESOURCES resources.qrc)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    binarysearchtree.cpp
    binarysearchtree.h
    treevisualizer.cpp
    treevisualizer.h
    ${PROJECT_RESOURCES}
)

qt_add_executable(BinarySearchTreeVisualization
    ${PROJECT_SOURCES}
)

target_link_libraries(BinarySearchTreeVisualization PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
