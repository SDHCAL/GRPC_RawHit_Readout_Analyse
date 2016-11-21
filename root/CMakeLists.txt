include($ENV{ROOTSYS}/cmake/modules/RootNewMacros.cmake)

get_property(dict_include_dir DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
list (REMOVE_ITEM dict_include_dir ${LCIO_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})


foreach(incdir ${dict_include_dir})
  FILE (GLOB ifiles ${incdir}/*.h)
  list (APPEND dict_headers  ${ifiles}) 
endforeach()
ROOT_GENERATE_DICTIONARY(my_dict ${dict_headers}  LINKDEF Linkdef.h)

add_library( ${PROJECT_NAME}_dict SHARED my_dict.cxx)
install (TARGETS ${PROJECT_NAME}_dict DESTINATION lib)
