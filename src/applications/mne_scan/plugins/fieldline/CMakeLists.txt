cmake_minimum_required(VERSION 3.14)
project(scan_fieldline LANGUAGES CXX)

add_subdirectory(fieldline_acqsystem)

#Handle qt uic, moc, rrc automatically
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED
    COMPONENTS Core Widgets Network Concurrent
)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED
    COMPONENTS Core Widgets Network Concurrent
)

set(HEADERS
    fieldline_global.h
    fieldline_definitions.h
    fieldline.h
    fieldline_view.h
    fieldline_view_chassis.h
    fieldline_view_sensor.h
    ipfinder.h
    ipfinder_network_unix.h
)

set(SOURCES
    fieldline_global.cpp
    fieldline.cpp
    fieldline_view.cpp
    fieldline_view_chassis.cpp
    fieldline_view_sensor.cpp
    ipfinder.cpp
    ipfinder_network_unix.cpp
)

set(UI_FILES
    formfiles/fieldline_view.ui
    formfiles/fieldline_view_chassis.ui
    formfiles/fieldline_view_sensor.ui
)

set(FILE_TO_UPDATE fieldline_global.cpp)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/fieldline.json)
# message("File fieldline.json exists.")
else()
# message("File fieldline.json does not exist.")
# message("Creating a new fieldline.json file.")
execute_process(COMMAND "echo \"\""
    WORKING_DIRECTORY ${CMAKE_C
    OUTPUT_FILE fieldline.json
)
endif()

set(SOURCE_PATHS ${SOURCES})

list(TRANSFORM SOURCE_PATHS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set_source_files_properties(${FILE_TO_UPDATE} PROPERTIES
    OBJECT_DEPENDS "${SOURCE_PATHS}"
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI_FILES})

set(FFTW_LIBS "")

if(USE_FFTW)
if(WIN32)
    set(FFTW_LIBS
        ${FFTW_DIR_LIBS}/libfftw3-3.dll
        ${FFTW_DIR_LIBS}/libfftw3f-3.dll
        ${FFTW_DIR_LIBS}/libfftwf3l-3.dll
    )
    target_include_directories(${PROJECT_NAME} PRIVATE ${FFTW_DIR_INCLUDE})
elseif(UNIX AND NOT APPLE)
    set(FFTW_LIBS ${FFTW_DIR_LIBS}/lib/libfftw3.so)
    target_include_directories(${PROJECT_NAME} PRIVATE ${FFTW_DIR_INCLUDE}/api)
endif()
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ../)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Concurrent
    eigen
    mne_disp
    mne_utils
    mne_fiff
    scDisp
    scShared
    scMeas
    ${FFTW_LIBS}
    fieldline_acqsystem
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    SCAN_FIELDLINE_PLUGIN MNE_GIT_HASH_SHORT="${MNE_GIT_HASH_SHORT}"
    MNE_GIT_HASH_LONG="${MNE_GIT_HASH_LONG}"
)

if(NOT BUILD_SHARED_LIBS)
target_compile_definitions(${PROJECT_NAME} PRIVATE STATICBUILD QT_STATICPLUGIN)
endif()

