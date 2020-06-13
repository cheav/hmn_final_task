cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

project(find_sum LANGUAGES CXX)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

qt5_wrap_cpp(MOC_WRAPPERS item.h game_model.h game_logic.h)
qt5_add_resources(QRC_WRAPPERS qml.qrc)

add_executable(${PROJECT_NAME} main.cpp item.cpp game_model.cpp game_logic.cpp
    ${MOC_WRAPPERS} ${QRC_WRAPPERS})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick)
