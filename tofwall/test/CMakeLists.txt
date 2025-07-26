##############################################################################
#   Copyright (C) 2017 GSI Helmholtzzentrum für Schwerionenforschung GmbH    #
#   Copyright (C) 2017-2025 Members of R3B Collaboration                     #
#                                                                            #
#             This software is distributed under the terms of the            #
#                 GNU General Public Licence (GPL) version 3,                #
#                    copied verbatim in the file "LICENSE".                  #
#                                                                            #
# In applying this license GSI does not waive the privileges and immunities  #
# granted to it by virtue of its status as an Intergovernmental Organization #
# or submit itself to any jurisdiction.                                      #
##############################################################################

if(GTEST_FOUND)
    add_executable(
        SofTofWUnitTests
        testSofTofWContainerPar.cxx)
    target_link_libraries(SofTofWUnitTests PRIVATE GTest::gtest_main GTest::gmock_main
                                                   R3BSofTofW)
    gtest_discover_tests(SofTofWUnitTests DISCOVERY_TIMEOUT 600)
endif(GTEST_FOUND)

generate_root_test_script(${R3BSOF_SOURCE_DIR}/tofwall/test/testSofTofWSimulation.C)
add_test(NAME SofTofWSimulation COMMAND ${R3BROOT_BINARY_DIR}/sofia/tofwall/test/testSofTofWSimulation.sh)
set_tests_properties(SofTofWSimulation PROPERTIES TIMEOUT "2000")
set_tests_properties(SofTofWSimulation PROPERTIES PASS_REGULAR_EXPRESSION
                                                  "Macro finished successfully.")
