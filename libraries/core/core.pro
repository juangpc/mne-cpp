#==============================================================================================================
#
# @file     core.pro
# @author   Juan GPC <jgarciaprieto@mgh.harvard.edu>
# @since    0.1.9
# @date     July, 2021
#
# @section  LICENSE
#
# Copyright (C) 2021, Juan GPC. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that
# the following conditions are met:
#     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
#       following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
#       the following disclaimer in the documentation and/or other materials provided with the distribution.
#     * Neither the name of MNE-CPP authors nor the names of its contributors may be used
#       to endorse or promote products derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
#
# @brief    This project file builds the core library.
#
#==============================================================================================================

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
    commandlineoptionsparser.cpp \

HEADERS += \
    commandlineoption.h \
    commandlineoptionsparser.h \
    core_global.h \

INCLUDEPATH += $${MNE_INCLUDE_DIR}

header_files.files = $${HEADERS}
header_files.path = $${MNE_INSTALL_INCLUDE_DIR}/core

INSTALLS += \
            header_files \

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
