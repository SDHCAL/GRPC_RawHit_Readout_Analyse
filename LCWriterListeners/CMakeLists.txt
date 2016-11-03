project(LCWriterListeners)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src)

set (${PROJECT_NAME}_sources ${${PROJECT_NAME}_sources} PARENT_SCOPE)
set (${PROJECT_NAME}_include ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
