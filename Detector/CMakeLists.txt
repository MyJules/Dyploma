cmake_minimum_required(VERSION 3.16)

project(Detector VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("src")

if(WIN32)
    # Specify OpenCV folder, and take care of dependenciesand includes
    set(OpenCV_DIR "libs/OpenCV-MinGW-Build-OpenCV-4.5.5-x64")
endif()
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Qt6 6.2 COMPONENTS Quick Multimedia Core5Compat REQUIRED)

qt_add_executable(appDetector
    src/main.cpp
    src/videoprocessor.h
    src/videoprocessor.cpp
    src/cvutils.h
    src/cvutils.cpp
)

qt_add_qml_module(appDetector
    URI Detector
    VERSION 1.0
    QML_FILES ui/main.qml
)

set_target_properties(appDetector PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appDetector
    PRIVATE
    Qt6::Quick
    Qt6::Multimedia
    ${OpenCV_LIBS}
)

install(TARGETS appDetector
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
