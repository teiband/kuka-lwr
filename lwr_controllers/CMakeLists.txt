cmake_minimum_required(VERSION 2.8.3)
project(lwr_controllers)

find_package(catkin REQUIRED COMPONENTS
  controller_interface
  control_msgs
  forward_command_controller
  control_toolbox
  realtime_tools
  urdf
  roscpp
  kdl_parser
  message_generation
  cmake_modules
  kdl_conversions
  sensor_msgs
  geometry_msgs
  std_msgs
  joint_impedance_interface
  cartesian_impedance_interface
)

include_directories(include ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS})

link_directories(${catkin_LIBRARY_DIRS})

add_message_files(
  FILES
  PoseRPY.msg
  RPY.msg
  MultiPriorityTask.msg
  CartesianImpedancePoint.msg
  Stiffness.msg
)
add_service_files(
  FILES
  SetCartesianImpedanceCommand.srv
)
generate_messages(
  DEPENDENCIES
  std_msgs
  geometry_msgs
  sensor_msgs
)

catkin_package(
  CATKIN_DEPENDS
    controller_interface
    control_msgs
    control_toolbox
    realtime_tools
    urdf
    forward_command_controller
    kdl_parser
    message_runtime
    joint_impedance_interface
    cartesian_impedance_interface
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
)

add_library(${PROJECT_NAME}
  src/cartesian_position.cpp
  src/itr_cartesian_impedance_controller.cpp
  src/itr_joint_impedance_controller.cpp
  src/joint_impedance_controller.cpp	
  src/inverse_dynamics_controller.cpp
  src/computed_torque_controller.cpp
  src/one_task_inverse_kinematics.cpp
  src/multi_task_priority_inverse_kinematics.cpp
  src/multi_task_priority_inverse_dynamics.cpp
  src/minimum_effort_inverse_dynamics.cpp
  src/backstepping_controller.cpp
  src/dynamic_sliding_mode_controller.cpp
  src/dynamic_sliding_mode_controller_task_space.cpp
  src/one_task_inverse_dynamics_jl.cpp
  src/gravity_compensation.cpp
  src/cartesian_impedance_controller.cpp
  src/lwr_state_controller.cpp
)

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_gencpp)

target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})


install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  )

install(FILES lwr_controllers_plugins.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
