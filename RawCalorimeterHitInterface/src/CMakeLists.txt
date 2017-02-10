#find_package(LCIO REQUIRED)

#include_directories(${LCIO_INCLUDE_DIRS})

add_executable(test_domain test_domain.cpp)
add_executable(test_RawCaloHitTools test_RawCaloHitTools.cpp RawCalorimeterHitUtilities.cpp)
target_link_libraries(test_RawCaloHitTools ${LCIO_LIBRARIES})
add_executable(test_RawSDHCALData test_RawSDHCALData.cpp RawCalorimeterHitUtilities.cpp RawHit_SDHCAL_Data.cpp)
target_link_libraries(test_RawSDHCALData ${LCIO_LIBRARIES})
add_executable(test_RawCaloHitClustering test_RawCaloHitClustering.cpp RawCalorimeterHitUtilities.cpp RawHit_SDHCAL_Data.cpp RawCalorimeterHitClustering.cpp TimeCounter.cpp)
target_link_libraries(test_RawCaloHitClustering ${LCIO_LIBRARIES})
add_executable(test_ASIC_Coordinate_Translator test_ASIC_Coordinate_Translator.cpp)


add_test(test_domain_runs test_domain)
add_test(test_RawCaloHitTools_runs test_RawCaloHitTools)
add_test(test_RawSDHCALData_runs test_RawSDHCALData)
add_test(test_RawCaloHitClustering_runs test_RawCaloHitClustering)
add_test(test_ASIC_Coordinate_Translator_runs test_ASIC_Coordinate_Translator)

set (${PROJECT_NAME}_sources  
 ${CMAKE_CURRENT_SOURCE_DIR}/RawHit_SDHCAL_Data.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/RawCalorimeterHitUtilities.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/RawCalorimeterHitClustering.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/TimeCounter.cpp
PARENT_SCOPE)




