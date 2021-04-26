cmake_minimum_required(VERSION 3.20)

project(qcustomplot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets PrintSupport REQUIRED)

if(MSVC)
    set(WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

add_library(qcustomplot SHARED)

target_compile_definitions(qcustomplot PRIVATE QCUSTOMPLOT_COMPILE_LIBRARY)
target_include_directories(qcustomplot 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/qcustomplot
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC
        ${Qt6Widgets_INCLUDE_DIRS}
        ${Qt6PrintSupport_INCLUDE_DIRS}
)
target_sources(qcustomplot 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include/qcustomplot/qcustomplot.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/qcustomplot.cpp
  )
target_link_libraries(qcustomplot 
    PUBLIC 
        Qt6::Widgets 
        Qt6::PrintSupport
)
