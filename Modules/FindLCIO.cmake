
FIND_PATH( LCIO_INCLUDE_DIR  NAMES lcio.h PATHS "$ENV{LCIO}/include" NO_DEFAULT_PATH)
if (NOT LCIO_INCLUDE_DIR)
   FIND_PATH( LCIO_INCLUDE_DIR  NAMES lcio.h PATHS "$ENV{LCIODIR}/include" NO_DEFAULT_PATH)
endif()
if (NOT LCIO_INCLUDE_DIR)
  FIND_PATH( LCIO_INCLUDE_DIR  NAMES lcio.h )
endif()


if( LCIO_INCLUDE_DIR )
    GET_FILENAME_COMPONENT( LCIO_ROOT ${LCIO_INCLUDE_DIR} PATH )
endif()


find_library( LCIO_LIBRARY NAMES lcio HINTS ${LCIO_ROOT}/lib )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LCIO_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LCIO  DEFAULT_MSG
                                  LCIO_LIBRARY LCIO_INCLUDE_DIR)

mark_as_advanced(LCIO_INCLUDE_DIR LCIO_LIBRARY )

set(LCIO_LIBRARIES ${LCIO_LIBRARY} )
set(LCIO_INCLUDE_DIRS ${LCIO_INCLUDE_DIR} )
