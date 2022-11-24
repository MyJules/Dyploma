cmake_minimum_required(VERSION 3.16)

project(Detector VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("src")

if(WIN32)
    # Specify OpenCV folder, and take care of dependenciesand includes
    set(OpenCV_DIR "libs/OpenCV-MinGW-Build-OpenCV-4.5.2-x64")
elseif(ANDROID)
    set(OpenCV_STATIC ON)
    set(CMAKE_INSTALL_LIBDIR "libs/")
    set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/OpenCV-android-sdk/sdk/native/jni")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/OpenCV-android-sdk/sdk/native/jni/include/")
    link_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/OpenCV-android-sdk/sdk/native/libs/armeabi-v7a/")
    link_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/OpenCV-android-sdk/sdk/native/3rdparty/libs/armeabi-v7a/")
    set(ANDROID_EXTRA_LIBS "${CMAKE_CURRENT_SOURCE_DIR}/libs/OpenCV-android-sdk/sdk/native/libs/armeabi-v7a/libopencv_java4.so" CACHE INTERNAL "")
else()
    set(OpenCV_DIR "libs/opencv")
endif()

# Qt library
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick Multimedia QuickControls2)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Multimedia QuickControls2)

# OpenCv library
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

message(STATUS " version: ${OpenCV_VERSION}")
message(STATUS " libraries: ${OpenCV_LIBS}")
message(STATUS " include path: ${OpenCV_INCLUDE_DIRS}")
message(STATUS " dir path: ${OpenCV_DIR}")

set(PROJECT_SOURCES
        src/main.cpp
        src/cvutils.h
        src/cvutils.cpp
        src/filters/videofilters.h
        src/filters/ivideofilter.h
        src/filters/ivideofilter.cpp
        src/filters/goodfeaturestotrack.h
        src/filters/goodfeaturestotrack.cpp
        qml.qrc
)

if(ANDROID)
    add_library(Detector SHARED
        ${PROJECT_SOURCES}
    )
else()
    add_executable(Detector
      ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(Detector
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
  Qt${QT_VERSION_MAJOR}::QuickControls2
  Qt${QT_VERSION_MAJOR}::Multimedia
  ${OpenCV_LIBS}
)

set_target_properties(Detector PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS Detector
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(Detector)
    qt_finalize_executable(Detector)
endif()
