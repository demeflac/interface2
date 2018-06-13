cmake_minimum_required(VERSION 3.1)
set(ignoreMe "${QT_QMAKE_EXECUTABLE}")

project(Sismica1)
set(CMAKE_CXX_STANDARD 14)
add_executable(${PROJECT_NAME} "sismica1.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/..")
