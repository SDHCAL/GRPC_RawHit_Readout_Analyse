if (${ROOT_VERSION} VERSION_LESS 6.20)
   include($ENV{ROOTSYS}/cmake/modules/RootNewMacros.cmake)
else()
   include($ENV{ROOTSYS}/cmake/RootMacros.cmake)
endif()
   
get_property(dict_include_dir DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#list (REMOVE_ITEM dict_include_dir ${LCIO_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})
list (REMOVE_ITEM dict_include_dir ${ROOT_INCLUDE_DIRS})
if (BUILD_AGAINST_CONFDB)
  list (REMOVE_ITEM dict_include_dir ${SDHCAL_ILCCONFDB_INCLUDE_DIRS})
  list (REMOVE_ITEM dict_include_dir ${XercesC_INCLUDE_DIRS})
endif()

foreach(incdir ${dict_include_dir})
  FILE (GLOB ifiles ${incdir}/*.h)
  list (APPEND dict_headers  ${ifiles}) 
endforeach()
ROOT_GENERATE_DICTIONARY(my_dict ${dict_headers}  LINKDEF Linkdef.h)

add_library( ${PROJECT_NAME}_dict SHARED my_dict.cxx)
install (TARGETS ${PROJECT_NAME}_dict DESTINATION lib)

  
if (NOT ${ROOT_VERSION} VERSION_LESS 6) 
  message ( "ROOT 6 needs extra stuff" )
  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/libmy_dict_rdict.pcm DESTINATION lib)
endif()

