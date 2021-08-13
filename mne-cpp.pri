############################################## GLOBAL FUNCTIONS ###############################################

# Define minQtVersion Test
defineTest(minQtVersion) {
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}

############################################### GLOBAL DEFINES ################################################

VERSION = 0.1.9

QMAKE_TARGET_PRODUCT = MNE-CPP
QMAKE_TARGET_DESCRIPTION = A Framework for Electrophysiology.
QMAKE_TARGET_COPYRIGHT = Copyright (C) 2020 Authors of MNE-CPP. All rights reserved.

########################################### PROJECT CONFIGURATION #############################################


# To disable/enable any of the sub-projects (i.e. tests, benchmarks, examples, etc.)
#    comment/uncomment that line in the MNE_CPP definition.

# Default flags
MNECPP_CONFIG += \
    applications    \
    testframes      \
    benchmarks      \
    examples        \
#    wasm            \
#    withCodeCov     \
#    useFFT          \
#    withAppBundles  \
#    noQOpenGLWidget \
#    trace           \
#    static          \

# Define c++ version
CONFIG += c++14

# Suppress untested SDK version checks on MacOS
macx {
    CONFIG += sdk_no_version_check
}

# Check versions
!minQtVersion(5, 10, 0) {
    error("You are trying to build with Qt version $${QT_VERSION}. However, the minimal Qt version to build MNE-CPP is 5.10.0.")
}

# Build static version if wasm flag was defined
contains(MNECPP_CONFIG, wasm) {
    message("The wasm flag was detected. Building static version of MNE-CPP. Disabling QOpenGLWidget support.")
    MNECPP_CONFIG += static noQOpenGLWidget
}

contains(MNECPP_CONFIG, static) {
    message("The static flag was detected. Building static version of MNE-CPP.")
}

contains(MNECPP_CONFIG, trace) {
    DEFINES += TRACE
}

########################################### DIRECTORY DEFINITIONS #############################################

# Repository directory
ROOT_DIR = $${PWD}

# Include directories
EIGEN_INCLUDE_DIR = $$EIGEN_INCLUDE_DIR
isEmpty(EIGEN_INCLUDE_DIR) {
    EIGEN_INCLUDE_DIR = $$shell_path($${PWD}/include/3rdParty/eigen3)
}

MNE_INCLUDE_DIR = $$MNE_INCLUDE_DIR
isEmpty( MNE_INCLUDE_DIR ) {
    MNE_INCLUDE_DIR = $$shell_path($${PWD}/libraries)
}

MNE_SCAN_INCLUDE_DIR = $$MNE_SCAN_INCLUDE_DIR
isEmpty( MNE_SCAN_INCLUDE_DIR ) {
    MNE_SCAN_INCLUDE_DIR = $$shell_path($${PWD}/applications/mne_scan/libs)
}

MNE_ANALYZE_INCLUDE_DIR = $$MNE_ANALYZE_INCLUDE_DIR
isEmpty( MNE_ANALYZE_INCLUDE_DIR ) {
    MNE_ANALYZE_INCLUDE_DIR = $$shell_path($${PWD}/applications/mne_analyze/libs)
}

# Library directories
MNE_LIBRARY_DIR = $$MNE_LIBRARY_DIR
isEmpty( MNE_LIBRARY_DIR ) {
    MNE_LIBRARY_DIR = $$shell_path($${PWD}/lib)
}

win32 {
    FFTW_DIR_LIBS = $$shell_path($${PWD}/include/3rdParty/fftw)
    FFTW_DIR_INCLUDE = $$shell_path($${PWD}/include/3rdParty/fftw)
}
unix{
    FFTW_DIR_LIBS = $$shell_path($${PWD}/include/3rdParty/fftw/lib)
    FFTW_DIR_INCLUDE = $$shell_path($${PWD}/include/3rdParty/fftw/include)
}

# Binary directory
MNE_BINARY_DIR = $$MNE_BINARY_DIR
isEmpty( MNE_BINARY_DIR ) {
    MNE_BINARY_DIR = $$shell_path($${PWD}/bin)
}

# Install directory
MNE_INSTALL_INCLUDE_DIR = $$MNE_INSTALL_INCLUDE_DIR
isEmpty( MNE_INSTALL_INCLUDE_DIR ) {
    MNE_INSTALL_INCLUDE_DIR = $$shell_path($${PWD}/include)
}

