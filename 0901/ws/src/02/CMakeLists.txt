cmake_minimum_required(VERSION 3.8)
project(02)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)


find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()


add_executable(Pub_node_image src/Pub.cpp)
add_executable(Sub_node_image src/Sub.cpp)
add_executable(Select src/Select.cpp)


ament_target_dependencies(Pub_node_image 
  rclcpp
  cv_bridge
  std_msgs
  sensor_msgs 
)

ament_target_dependencies(Sub_node_image 
  rclcpp
  cv_bridge
  std_msgs
  sensor_msgs 
)

target_link_libraries(Pub_node_image 
  ${OpenCV_LIBRARIES}
)

target_link_libraries(Sub_node_image 
  ${OpenCV_LIBRARIES}
)

target_link_libraries(Select 
  ${OpenCV_LIBRARIES}
)

target_include_directories(Pub_node_image PUBLIC
  ${OpenCV_INCLUDE_DIRS}
)

target_include_directories(Sub_node_image PUBLIC
  ${OpenCV_INCLUDE_DIRS}
)

target_include_directories(Select PUBLIC
  ${OpenCV_INCLUDE_DIRS}
)

# 安装 launch 文件
install(DIRECTORY launch
    DESTINATION share/${PROJECT_NAME}
)
install(TARGETS 
  Pub_node_image
  Sub_node_image
  Select
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()