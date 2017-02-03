
add_executable(test_GG_counter test_GG_counter.cpp)
add_executable(test_RawHit_standard_merge_predicate test_RawHit_standard_merge_predicate.cpp  RawHit_standard_merge_predicate.cpp ${RawCalorimeterHitInterface_sources}  ${ExperimentalSetup_sources})
target_link_libraries(test_RawHit_standard_merge_predicate ${LCIO_LIBRARIES})

add_test(test_GG_counter_runs test_GG_counter)
add_test(test_RawHit_standard_merge_predicate_runs test_RawHit_standard_merge_predicate)

set (${PROJECT_NAME}_sources  
 ${CMAKE_CURRENT_SOURCE_DIR}/GG_counter_histos.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/RawHit_SDHCAL_Data_Reader_From_LCEvent.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/RawHit_SDHCAL_Data_Reader_FromBIF.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/RawHit_SDHCAL_Data_Reader_FromTimeAnalysis.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/RawHit_SDHCAL_Data_Reader_Noise.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/RawHit_SDHCAL_Data_Reader_Trivent.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/RawHit_SDHCAL_Data_Reader_Threshold_Filter.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/RawHit_SDHCAL_Data_Reader_TriggeredMode.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/RawHit_standard_merge_predicate.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/RawHit_SDHCAL_Data_Reader_Clusterer.cpp
PARENT_SCOPE)




