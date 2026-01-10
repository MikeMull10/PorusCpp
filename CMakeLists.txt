cmake_minimum_required(VERSION 3.16)
project(PorusCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/Widgets/*.cpp"
)

qt_add_executable(PorusCpp
    ${SRC_FILES}
)

target_link_libraries(PorusCpp
    PRIVATE Qt6::Widgets
)

target_include_directories(PorusCpp PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Widgets
)