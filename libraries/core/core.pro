include(../../mne-cpp.pri)

TEMPLATE = lib

CONFIG += skip_target_version_ext
CONFIG += c++17

QT += network concurrent
QT -= gui

DEFINES += CORE_LIBRARY

DESTDIR = $${MNE_LIBRARY_DIR}

TARGET = Core
TARGET = $$join(TARGET,,mnecpp,)
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}

contains(MNECPP_CONFIG, static) {
    CONFIG += staticlib
    DEFINES += STATICBUILD
} else {
    CONFIG += shared
}

LIBS += -L$${MNE_LIBRARY_DIR}
CONFIG(debug, debug|release) {
    LIBS +=
} else {
    LIBS +=
}

SOURCES += \
    commandlineoption.cpp \
    commandlineoptionsparser.cpp

HEADERS += \
    commandlineoption.h \
    commandlineoptionsparser.h \
    core_global.h

INCLUDEPATH += $${MNE_INCLUDE_DIR}

header_files.files = $${HEADERS}
header_files.path = $${MNE_INSTALL_INCLUDE_DIR}/core

INSTALLS += header_files

contains(MNECPP_CONFIG, withCodeCov) {
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}

win32:!contains(MNECPP_CONFIG, static) {
    QMAKE_POST_LINK += $$QMAKE_COPY $$shell_path($${MNE_LIBRARY_DIR}/$${TARGET}.dll) $${MNE_BINARY_DIR}
}

macx {
    QMAKE_LFLAGS_SONAME = -Wl,-install_name,@rpath/
}
