cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(myapp main.cpp)

target_include_directories(myapp PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(myapp ${OpenCV_LIBS})