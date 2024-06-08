cmake_minimum_required(VERSION 3.8)
project(testnode)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
add_executable(maintestnode src/maintestnode.cpp)
add_executable(talker src/talker.cpp)
add_executable(driveenabler src/driveenabler.cpp)
add_executable(servosub src/servosub.cpp)
ament_target_dependencies(maintestnode rclcpp std_msgs geometry_msgs)
ament_target_dependencies(talker rclcpp std_msgs geometry_msgs)
ament_target_dependencies(driveenabler rclcpp std_msgs geometry_msgs)
ament_target_dependencies(servosub rclcpp std_msgs geometry_msgs)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

install(TARGETS
  maintestnode
  talker
  driveenabler
  servosub
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
