cmake_minimum_required(VERSION 3.10)
project(Opencv-4)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

# 添加可执行文件
add_executable(Test413 src/Test413.cpp)

# 链接 OpenCV 库（目标名和上面一致）
target_link_libraries(Test413 ${OpenCV_LIBS})

# 可选：打印 OpenCV 信息
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include: ${OpenCV_INCLUDE_DIRS}")