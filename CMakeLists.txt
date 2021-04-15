cmake_minimum_required(VERSION 3.12)

project(gw-dss-cpp LANGUAGES CXX)

# ====================================================================================================
#                                           GLOBAL VARIABLES
# ====================================================================================================

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS src/view)


# ====================================================================================================
#                                            DEPENDENCIES
# ====================================================================================================

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools Sql PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools Sql PrintSupport REQUIRED)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/extern/qtm-calc EXCLUDE_FROM_ALL)

add_library(qcustomplot SHARED)
target_sources(qcustomplot PRIVATE ${CMAKE_CURRENT_LIST_DIR}/3rd_party/qcustomplot/qcustomplot.cpp)
target_link_libraries(qcustomplot PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport)
target_include_directories(qcustomplot PRIVATE ${CMAKE_CURRENT_LIST_DIR}/3rd_party/qcustomplot)

# ====================================================================================================
#                                        TRANSLATIONS & SOURCES
# ====================================================================================================

set(TS_FILES
    ${CMAKE_CURRENT_LIST_DIR}/src/translations/gw-dss-cpp_en_US.ts
    ${CMAKE_CURRENT_LIST_DIR}/src/translations/gw-dss-cpp_uk_UA.ts
    ${CMAKE_CURRENT_LIST_DIR}/src/translations/gw-dss-cpp_ru_RU.ts
)

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


# ====================================================================================================
#                                            BUILD RULES
# ====================================================================================================

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gw-dss-cpp WIN32
        ${PROJECT_SOURCES}
    )

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
endif()

set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/translations")
qt_add_translation(QM_FILES ${TS_FILES})
add_custom_target(translations ALL DEPENDS ${QM_FILES})

add_dependencies(gw-dss-cpp libqtmcalc qcustomplot)
target_link_libraries(gw-dss-cpp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql libqtmcalc qcustomplot)

# ====================================================================================================
#                                             DEPLOYMENT
# ====================================================================================================

add_custom_target(deployment)

# windows deployment
if(WIN32 AND TARGET deployment)
    add_custom_command(TARGET gw-dss-cpp
        POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/windeployqt"

        COMMAND windeployqt --dir "${CMAKE_BINARY_DIR}/windeployqt" --no-translations --release $<TARGET_FILE:gw-dss-cpp>
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_BINARY_DIR}/translations" "${CMAKE_CURRENT_BINARY_DIR}/windeployqt/translations"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:gw-dss-cpp>" "${CMAKE_CURRENT_BINARY_DIR}/windeployqt/$<TARGET_FILE_NAME:gw-dss-cpp>"

        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:libqtmcalc>" "${CMAKE_CURRENT_BINARY_DIR}/windeployqt/$<TARGET_FILE_NAME:libqtmcalc>"
    )
endif()

# ====================================================================================================
#                                         UPDATE TRANSLATIONS
# ====================================================================================================

add_custom_target(make-lupdate 
    COMMAND lupdate ${CMAKE_CURRENT_LIST_DIR}/src/ -ts ${TS_FILES}
)
