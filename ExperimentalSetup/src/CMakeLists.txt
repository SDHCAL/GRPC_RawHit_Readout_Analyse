
add_executable(test_DIFdrivenDevice test_DIFdrivenDevice.cpp PadDevice.cpp BifaceStripDevice.cpp)
add_test(test_DIFdrivenDevice_runs test_DIFdrivenDevice)

set (${PROJECT_NAME}_sources 
  ${CMAKE_CURRENT_SOURCE_DIR}/BifaceStripDevice.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/PadDevice.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/ExperimentalSetup.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/ConfigInfoExceptions.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/ConfigInfo.cpp 
PARENT_SCOPE)



