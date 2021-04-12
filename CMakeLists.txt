cmake_minimum_required(VERSION 3.12)

project(gw-dss-cpp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(DEPLOY_TOOL windeployqt)
    set(EXECUTABLE_SUFFIX .exe)
endif()

if(UNIX AND NOT APPLE)
    set(DEPLOY_TOOL linuxdeployqt)
    set(EXECUTABLE_SUFFIX "")
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools Sql REQUIRED)

set(TS_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/translations/gw-dss-cpp_en_US.ts
    ${CMAKE_CURRENT_LIST_DIR}/src/translations/gw-dss-cpp_uk_UA.ts
    ${CMAKE_CURRENT_LIST_DIR}/src/translations/gw-dss-cpp_ru_RU.ts
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
    qt_add_executable(gw-dss-cpp WIN32
        ${PROJECT_SOURCES}
    )

    set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/translations")
    qt_add_translation(QM_FILES ${TS_FILES})
    add_custom_target(translations ALL DEPENDS ${QM_FILES})
else()
    if(ANDROID)
        add_library(gw-dss-cpp SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(gw-dss-cpp WIN32
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(gw-dss-cpp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql libqtmcalc)

add_custom_command(TARGET gw-dss-cpp
    POST_BUILD

    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/${DEPLOY_TOOL}"
    COMMAND ${DEPLOY_TOOL} --dir "${CMAKE_BINARY_DIR}/${DEPLOY_TOOL}" --no-translations --release gw-dss-cpp${EXECUTABLE_SUFFIX}
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_BINARY_DIR}/translations" "${CMAKE_CURRENT_BINARY_DIR}/${DEPLOY_TOOL}/translations"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/gw-dss-cpp${EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_BINARY_DIR}/${DEPLOY_TOOL}/gw-dss-cpp${EXECUTABLE_SUFFIX}"

    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:libqtmcalc>" "${CMAKE_CURRENT_BINARY_DIR}/${DEPLOY_TOOL}/$<TARGET_FILE_NAME:libqtmcalc>"
)
