cmake_minimum_required(VERSION 3.8)
project(01)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)

# ✅ 关键修改：明确查找 OpenCV 的 highgui 模块
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# 可执行文件
add_executable(Pub_node src/Pub.cpp)
add_executable(Sub_node src/Sub.cpp)

# ✅ 依赖
ament_target_dependencies(Pub_node 
  rclcpp
  cv_bridge
  std_msgs
  sensor_msgs 
)

ament_target_dependencies(Sub_node 
  rclcpp
  cv_bridge
  std_msgs
  sensor_msgs 
)

# ✅ 链接 OpenCV（包括所有模块）
target_link_libraries(Pub_node 
  ${OpenCV_LIBRARIES}
)

target_link_libraries(Sub_node 
  ${OpenCV_LIBRARIES}
)

# ✅ 包含头文件
target_include_directories(Pub_node PUBLIC
  ${OpenCV_INCLUDE_DIRS}
)

target_include_directories(Sub_node PUBLIC
  ${OpenCV_INCLUDE_DIRS}
)

install(TARGETS 
  Pub_node
  Sub_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()