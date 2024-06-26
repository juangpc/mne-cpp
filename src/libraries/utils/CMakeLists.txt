cmake_minimum_required(VERSION 3.14)
project(mne_utils
  LANGUAGES CXX
)

#Handle qt uic, moc, rrc automatically
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(QT_REQUIRED_COMPONENTS Core Concurrent Gui)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${QT_REQUIRED_COMPONENTS})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_REQUIRED_COMPONENTS})

set(SOURCES
  file.cpp
  kmeans.cpp
  mnemath.cpp
  ioutils.cpp
  layoutloader.cpp
  layoutmaker.cpp
  selectionio.cpp
  spectrogram.cpp
  warp.cpp
  sphere.cpp
  generics/observerpattern.cpp
  generics/applicationlogger.cpp
  spectral.cpp
  mnetracer.cpp
  utils_global.cpp
)

set(HEADERS
  buildinfo.h
  file.h
  kmeans.h
  utils_global.h
  mnemath.h
  ioutils.h
  layoutloader.h
  layoutmaker.h
  selectionio.h
  spectrogram.h
  warp.h
  sphere.h
  simplex_algorithm.h
  generics/circularbuffer.h
  generics/commandpattern.h
  generics/observerpattern.h
  generics/applicationlogger.h
  spectral.h
  mnetracer.h
)

set(FILE_TO_UPDATE utils_global.cpp)

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

target_link_libraries(${PROJECT_NAME} PRIVATE
  ${QT_REQUIRED_COMPONENT_LIBS}
  eigen
  ${FFTW_LIBS})

target_compile_definitions(${PROJECT_NAME} PRIVATE MNE_UTILS_LIBRARY MNE_GIT_HASH_SHORT="${MNE_GIT_HASH_SHORT}" MNE_GIT_HASH_LONG="${MNE_GIT_HASH_LONG}")

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(${PROJECT_NAME} PRIVATE STATICBUILD)
endif()
