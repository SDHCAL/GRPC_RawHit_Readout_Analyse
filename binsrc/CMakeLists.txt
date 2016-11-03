
message( "ROOT_LIBS ${ROOT_LIBRARIES}")

add_executable(CMSTree_Producer CMSTree_Producer.cpp)
target_link_libraries(CMSTree_Producer  ${PROJECT_NAME} ${LCIO_LIBRARIES} ${ROOT_LIBRARIES})

install (TARGETS CMSTree_Producer DESTINATION bin)
