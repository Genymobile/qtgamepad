# Copyright (C) 2024 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(simple LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/universalinput/consolejoystickmonitor")

find_package(Qt6 REQUIRED COMPONENTS Core Gui UniversalInput Gamepad)

qt_add_executable(simple
    main.cpp
    gamepadmonitor.h
    gamepadmonitor.cpp
)

set_target_properties(simple PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(simple PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Gamepad
)

#DISTFILES += android/AndroidManifest.xml
#ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
#target.path = $$[QT_INSTALL_EXAMPLES]/gamepad/simple

install(TARGETS simple
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)


