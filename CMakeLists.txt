cmake_minimum_required(VERSION 3.12)

project(gw-dss-cpp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools Sql REQUIRED)

set(TS_FILES
        ${CMAKE_CURRENT_LIST_DIR}/src/translation/gw-dss-cpp_en_US.ts
        ${CMAKE_CURRENT_LIST_DIR}/src/translation/gw-dss-cpp_uk_UA.ts
        ${CMAKE_CURRENT_LIST_DIR}/src/translation/gw-dss-cpp_ru_RU.ts
)

set(CMAKE_AUTOUIC_SEARCH_PATHS src/view)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/extern/qtm-calc EXCLUDE_FROM_ALL)


set(PROJECT_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/controllers/hpp/mainwindow.h
        ${CMAKE_CURRENT_LIST_DIR}/src/controllers/hpp/verticatextdelegate.h
        ${CMAKE_CURRENT_LIST_DIR}/src/analysis/hpp/dbapi.h
        ${CMAKE_CURRENT_LIST_DIR}/src/analysis/hpp/dataprocessing.h
        ${CMAKE_CURRENT_LIST_DIR}/src/analysis/hpp/predict.h

        ${CMAKE_CURRENT_LIST_DIR}/src/controllers/cpp/mainwindow.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/controllers/cpp/verticatextdelegate.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/analysis/cpp/dbapi.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/analysis/cpp/dataprocessing.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/analysis/cpp/predict.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/view/mainwindow.ui

        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gw-dss-cpp
        ${PROJECT_SOURCES}
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(gw-dss-cpp SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(gw-dss-cpp
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(gw-dss-cpp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql libqtmcalc)
