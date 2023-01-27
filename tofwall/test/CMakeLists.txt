


generate_root_test_script(${R3BSOF_SOURCE_DIR}/tofwall/test/testSofTofWSimulation.C)
add_test(SofTofWUnitTests ${R3BROOT_BINARY_DIR}/sofia/tofwall/test/testSofTofWSimulation.sh)
set_tests_properties(SofTofWUnitTests PROPERTIES TIMEOUT "2000")
set_tests_properties(SofTofWUnitTests PROPERTIES PASS_REGULAR_EXPRESSION
                                                  "Macro finished successfully.")
