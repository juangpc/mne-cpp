cmake_minimum_required(VERSION 3.14)
project(mne_connectivity LANGUAGES CXX)

#Handle qt uic, moc, rrc automatically
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(QT_REQUIRED_COMPONENTS Core Concurrent Network)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_REQUIRED_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_REQUIRED_COMPONENTS})

set(SOURCES
    connectivity_global.cpp
    metrics/abstractmetric.cpp
    metrics/correlation.cpp
    metrics/crosscorrelation.cpp
    metrics/coherency.cpp
    metrics/coherence.cpp
    metrics/imagcoherence.cpp
    metrics/unbiasedsquaredphaselagindex.cpp
    metrics/phaselockingvalue.cpp
    metrics/weightedphaselagindex.cpp
    metrics/debiasedsquaredweightedphaselagindex.cpp
    metrics/phaselagindex.cpp
    network/network.cpp
    network/networknode.cpp
    network/networkedge.cpp
    connectivitysettings.cpp
    connectivity.cpp
)

set(HEADERS
    connectivity_global.h
    metrics/abstractmetric.h
    metrics/correlation.h
    metrics/crosscorrelation.h
    metrics/coherency.h
    metrics/coherence.h
    metrics/imagcoherence.h
    metrics/unbiasedsquaredphaselagindex.h
    metrics/phaselockingvalue.h
    metrics/weightedphaselagindex.h
    metrics/debiasedsquaredweightedphaselagindex.h
    metrics/phaselagindex.h
    network/network.h
    network/networknode.h
    network/networkedge.h
    connectivitysettings.h
    connectivity.h
)

set(FILE_TO_UPDATE connectivity_global.cpp)

set(SOURCE_PATHS ${SOURCES})
list(TRANSFORM SOURCE_PATHS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set_source_files_properties(${FILE_TO_UPDATE} PROPERTIES OBJECT_DEPENDS "${SOURCE_PATHS}")

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

set(FFTW_LIBS "")

if(USE_FFTW)
  if (WIN32)
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

set(QT_REQUIRED_COMPONENT_LIBS ${QT_REQUIRED_COMPONENTS})
list(TRANSFORM QT_REQUIRED_COMPONENT_LIBS PREPEND "Qt${QT_VERSION_MAJOR}::")

set(MNE_LIBS_REQUIRED 
    mne_utils
    mne_fiff
    mne_fs
    mne_mne
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${QT_REQUIRED_COMPONENT_LIBS}
    ${MNE_LIBS_REQUIRED}
    eigen
    ${FFTW_LIBS})

target_compile_definitions(${PROJECT_NAME} PRIVATE MNE_CONNECTIVITY_LIBRARY MNE_GIT_HASH_SHORT="${MNE_GIT_HASH_SHORT}" MNE_GIT_HASH_LONG="${MNE_GIT_HASH_LONG}")

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PRIVATE STATICBUILD)
endif()
