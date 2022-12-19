
enable_testing()
set(PROJECT_TEST_NAME SofTofWUnitTests)
set(GTEST_ROOT ${SIMPATH})
find_package(GTest)

if(GTEST_FOUND)
file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/sofia/tofwall/test/*.cxx)

include_directories(
    ${GTEST_INCLUDE_DIRS}
    ${SYSTEM_INCLUDE_DIRECTORIES}
    ${BASE_INCLUDE_DIRECTORIES}
    ${R3BROOT_SOURCE_DIR}/r3bbase
    ${R3BROOT_SOURCE_DIR}/r3bdata
    ${R3BSOF_SOURCE_DIR}/sofdata/tofwData
    ${R3BSOF_SOURCE_DIR}/tofwall
    ${R3BSOF_SOURCE_DIR}/tofwall/sim
    ${R3BSOF_SOURCE_DIR}/tofwall/pars
    ${R3BSOF_SOURCE_DIR}/tofwall/digi
    )

link_directories(${GTEST_LIBS_DIR} ${ROOT_LIBRARY_DIR} ${FAIRROOT_LIBRARY_DIR}
                 ${Boost_LIBRARY_DIRS})

set(TEST_DEPENDENCIES
    ${GTEST_BOTH_LIBRARIES}
    ${ROOT_LIBRARIES}
    FairTools
    R3BData
    R3BSofData
    R3BSofTofW
    ParBase
    GeoBase)

add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
target_link_libraries(${PROJECT_TEST_NAME} ${TEST_DEPENDENCIES})
add_test(${PROJECT_TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_TEST_NAME})
endif(GTEST_FOUND)

generate_root_test_script(${R3BSOF_SOURCE_DIR}/tofwall/test/testSofTofWSimulation.C)
add_test(SofTofWUnitTests ${R3BROOT_BINARY_DIR}/sofia/tofwall/test/testSofTofWSimulation.sh)
set_tests_properties(SofTofWUnitTests PROPERTIES TIMEOUT "2000")
set_tests_properties(SofTofWUnitTests PROPERTIES PASS_REGULAR_EXPRESSION
                                                  "Macro finished successfully.")
