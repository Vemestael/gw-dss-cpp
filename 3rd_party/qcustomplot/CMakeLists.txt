cmake_minimum_required(VERSION 3.12)

project(qcustomplot LANGUAGES CXX)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets PrintSupport REQUIRED)

add_library(qcustomplot SHARED)
target_sources(qcustomplot PRIVATE ${CMAKE_CURRENT_LIST_DIR}/qcustomplot.cpp)
target_link_libraries(qcustomplot PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport)
target_include_directories(qcustomplot PUBLIC ${CMAKE_CURRENT_LIST_DIR})