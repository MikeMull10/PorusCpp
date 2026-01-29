cmake_minimum_required(VERSION 3.16)
project(PorusCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(OpenCV_DIR "F:/OpenCV/build/x64/vc16/lib/")
find_package(OpenCV REQUIRED)

qt_standard_project_setup()

file(GLOB_RECURSE SRC_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.h"
    "${CMAKE_SOURCE_DIR}/src/Widgets/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/Widgets/*.h"
    "${CMAKE_SOURCE_DIR}/src/Pages/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/Pages/*.h"
    "${CMAKE_SOURCE_DIR}/src/Algorithm/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/Algorithm/*.h"
    "${CMAKE_SOURCE_DIR}/src/Components/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/Components/*.h"
)

qt_add_resources(RESOURCE_FILES src/resources/resources.qrc)

qt_add_executable(PorusCpp
    ${SRC_FILES}
    ${RESOURCE_FILES}
)

target_link_libraries(PorusCpp
    PRIVATE Qt6::Widgets
)

target_link_libraries(PorusCpp PRIVATE ${OpenCV_LIBS})
target_include_directories(PorusCpp PRIVATE ${OpenCV_INCLUDE_DIRS})

target_include_directories(PorusCpp PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Widgets
    ${CMAKE_SOURCE_DIR}/src/Pages
    ${CMAKE_SOURCE_DIR}/src/resources
    ${CMAKE_SOURCE_DIR}/src/Algorithm
    ${CMAKE_SOURCE_DIR}/src/Components
)
