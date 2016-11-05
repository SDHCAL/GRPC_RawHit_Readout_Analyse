cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "$ENV{ROOTSYS}/etc/cmake")


#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions("-std=c++11")

option(GENERATE_SWIG "Flag to toggle python module generation" ON)

enable_testing()

project(GRPC_RawHit_Readout_Analyser)

set (CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

set( ${PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_PATCH 0 )


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

if (GENERATE_SWIG)
 add_subdirectory(swig)
endif()

install (TARGETS ${PROJECT_NAME} DESTINATION lib)
