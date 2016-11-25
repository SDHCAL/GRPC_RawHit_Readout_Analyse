
message( "ROOT_LIBS ${ROOT_LIBRARIES}")

add_executable(CMSTree_Producer CMSTree_Producer.cpp)
target_link_libraries(CMSTree_Producer  ${PROJECT_NAME} ${LCIO_LIBRARIES} ${ROOT_LIBRARIES})

install (TARGETS CMSTree_Producer DESTINATION bin)

if (BUILD_AGAINST_CONFDB)
  add_executable(dumpSetup dumpSetup.cpp)
  target_link_libraries(dumpSetup ${SDHCAL_ILCCONFDB_LIBRARIES} ${XercesC_LIBRARIES} )
  install (TARGETS dumpSetup DESTINATION bin)
endif()
