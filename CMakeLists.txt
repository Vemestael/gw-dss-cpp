cmake_minimum_required(VERSION 3.20)

project(gw-dss-cpp LANGUAGES CXX)

# ====================================================================================================
#                                            DEPENDENCIES
# ====================================================================================================

find_package(Qt6 COMPONENTS Widgets Sql LinguistTools REQUIRED)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/qcustomplot EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/qtm-calc-shared EXCLUDE_FROM_ALL)


# ====================================================================================================
#                                           GLOBAL VARIABLES
# ====================================================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/view)


# ====================================================================================================
#                                        TRANSLATIONS & SOURCES
# ====================================================================================================

set(PROJECT_UIS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/view/mainwindow.ui
)

set(PROJECT_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gw-dss-cpp/analysis/dataprocessing.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gw-dss-cpp/analysis/dbapi.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gw-dss-cpp/analysis/predict.h

    ${CMAKE_CURRENT_SOURCE_DIR}/include/gw-dss-cpp/controllers/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gw-dss-cpp/controllers/verticaltextdelegate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gw-dss-cpp/controllers/chartwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/gw-dss-cpp/controllers/errorwindow.h
)

set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/analysis/dbapi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/analysis/dataprocessing.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/analysis/predict.cpp 

    ${CMAKE_CURRENT_SOURCE_DIR}/src/controllers/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/controllers/verticaltextdelegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/controllers/chartwindow.cpp    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/controllers/errorwindow.cpp    
)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/translations/gw-dss-cpp_en_US.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/src/translations/gw-dss-cpp_uk_UA.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/src/translations/gw-dss-cpp_ru_RU.ts
)

set(PROJECT_RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/resources.qrc
)

set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_BINARY_DIR}/translations)
qt_add_translation(QM_FILES ${TS_FILES})
add_custom_target(translations ALL DEPENDS ${QM_FILES})


# ====================================================================================================
#                                            BUILD RULES
# ====================================================================================================

add_executable(gw-dss-cpp WIN32)
add_dependencies(gw-dss-cpp qcustomplot libqtmcalc)

target_compile_definitions(gw-dss-cpp PRIVATE QCUSTOMPLOT_USE_LIBRARY)
target_include_directories(gw-dss-cpp 
    PRIVATE
        ${Qt6Widgets_INCLUDE_DIRS} 
        ${Qt6Sql_INCLUDE_DIRS} 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_sources(gw-dss-cpp 
    PRIVATE 
        ${PROJECT_UIS} 
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES} 
        ${PROJECT_RESOURCES} 
        ${TS_FILES}
)
target_link_libraries(gw-dss-cpp 
    PRIVATE 
        Qt6::Widgets
        Qt6::Sql
        qcustomplot
        libqtmcalc    
)


# ====================================================================================================
#                                             DEPLOYMENT
# ====================================================================================================

# windows deployment
if(WIN32)
    add_custom_target(deployment
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/windeployqt"

        COMMAND windeployqt --dir "${CMAKE_BINARY_DIR}/windeployqt" --no-translations --release $<TARGET_FILE:gw-dss-cpp>
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_BINARY_DIR}/translations" "${CMAKE_CURRENT_BINARY_DIR}/windeployqt/translations"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:gw-dss-cpp>" "${CMAKE_CURRENT_BINARY_DIR}/windeployqt/$<TARGET_FILE_NAME:gw-dss-cpp>"

        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:libqtmcalc>" "${CMAKE_CURRENT_BINARY_DIR}/windeployqt/$<TARGET_FILE_NAME:libqtmcalc>"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:qcustomplot>" "${CMAKE_CURRENT_BINARY_DIR}/windeployqt/$<TARGET_FILE_NAME:qcustomplot>"
    )
    add_dependencies(deployment gw-dss-cpp translations)
endif()


# ====================================================================================================
#                                         UPDATE TRANSLATIONS
# ====================================================================================================

add_custom_target(make-lupdate 
    COMMAND lupdate ${CMAKE_CURRENT_SOURCE_DIR}/include/ ${CMAKE_CURRENT_SOURCE_DIR}/src/ -ts ${TS_FILES}
)
