cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "$ENV{ROOTSYS}/etc/cmake")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions("-std=c++11")

option(GENERATE_ROOT_DICT "Flag to toggle ROOT dictionnary genaration" ON)
option(BUILD_AGAINST_CONFDB "If ON, include code depending on configuration database code" ON)

enable_testing()

project(GRPC_RawHit_Readout_Analyser)

set (CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

set( ${PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_PATCH 0 )

if (BUILD_AGAINST_CONFDB)
  #ILCCONFDB need Xerces-c
  if (CMAKE_VERSION   VERSION_GREATER 3.1)
    include(FindXercesC)
    find_package(XercesC REQUIRED)
  else()
    set(XercesC_INCLUDE_DIRS /usr/include/xercesc/)
    find_library(XercesC_LIBRARIES xerces-c)
  endif()
  message ("XercesC ${XercesC_INCLUDE_DIRS} et ${XercesC_LIBRARIES}")
  find_package(SDHCAL_ILCCONFDB REQUIRED)
  include_directories(${SDHCAL_ILCCONFDB_INCLUDE_DIRS})
endif()

find_package(LCIO REQUIRED)
include_directories(${LCIO_INCLUDE_DIRS})

find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})


add_subdirectory(RawCalorimeterHitInterface)
include_directories(${RawCalorimeterHitInterface_include})

add_subdirectory(ExperimentalSetup)
include_directories(${ExperimentalSetup_include})

add_subdirectory(DataReaders)
include_directories(${DataReaders_include})

add_subdirectory(DataListeners)
include_directories(${DataListeners_include})

add_subdirectory(ConcreteExperiments)
include_directories(${ConcreteExperiments_include})

add_subdirectory(LCWriterListeners)
include_directories(${LCWriterListeners_include})

add_library( ${PROJECT_NAME} SHARED ${RawCalorimeterHitInterface_sources} ${ExperimentalSetup_sources} ${DataReaders_sources} ${DataListeners_sources} ${ConcreteExperiments_sources} ${LCWriterListeners_sources})

add_subdirectory(binsrc)


if (GENERATE_ROOT_DICT)
  add_subdirectory(root)
endif()


install (TARGETS ${PROJECT_NAME} DESTINATION lib)
