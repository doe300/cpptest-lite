
function(cpptest_discover_tests TARGET)
  cmake_parse_arguments(CPPTEST_DISCOVER "" "WORKING_DIRECTORY" "PROPERTIES;DISABLED_TESTS;FAILING_TESTS" ${ARGN})

  set(CPPTEST_TEST_DISCOVERY_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_cpptest_test_discovery.cmake")
  get_property(CPPTEST_MULTI_CONFIG_BUILD GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
  if(CPPTEST_MULTI_CONFIG_BUILD)
    set(CPPTEST_TEST_DISCOVERY_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_cpptest_test_discovery_$<CONFIG>.cmake")
    set_property(DIRECTORY APPEND PROPERTY TEST_INCLUDE_FILES "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_cpptest_test_discovery_${CTEST_CONFIGURATION_TYPE}.cmake")
  else()
    set_property(DIRECTORY APPEND PROPERTY TEST_INCLUDE_FILES "${CPPTEST_TEST_DISCOVERY_FILE}")
  endif()
  set(CPPTEST_TESTS_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_cpptest_tests.cmake")

  set(CPPTEST_TEST_DISCOVERY_EXTRA_ARGS "")
  if (CPPTEST_DISCOVER_WORKING_DIRECTORY)
    list(APPEND CPPTEST_TEST_DISCOVERY_EXTRA_ARGS "WORKING_DIRECTORY")
    list(APPEND CPPTEST_TEST_DISCOVERY_EXTRA_ARGS "${CPPTEST_DISCOVER_WORKING_DIRECTORY}")
  endif()
  if(CPPTEST_DISCOVER_PROPERTIES)
    list(APPEND CPPTEST_TEST_DISCOVERY_EXTRA_ARGS "PROPERTIES")
    list(APPEND CPPTEST_TEST_DISCOVERY_EXTRA_ARGS "${CPPTEST_DISCOVER_PROPERTIES}")
  endif()
  if(CPPTEST_DISCOVER_DISABLED_TESTS)
    list(APPEND CPPTEST_TEST_DISCOVERY_EXTRA_ARGS "DISABLED_TESTS")
    list(APPEND CPPTEST_TEST_DISCOVERY_EXTRA_ARGS "${CPPTEST_DISCOVER_DISABLED_TESTS}")
  endif()
  if(CPPTEST_DISCOVER_FAILING_TESTS)
    list(APPEND CPPTEST_TEST_DISCOVERY_EXTRA_ARGS "FAILING_TESTS")
    list(APPEND CPPTEST_TEST_DISCOVERY_EXTRA_ARGS "${CPPTEST_DISCOVER_FAILING_TESTS}")
  endif()

  string(REPLACE ";" " " CPPTEST_TEST_DISCOVERY_EXTRA_ARGS "${CPPTEST_TEST_DISCOVERY_EXTRA_ARGS}")
  file(GENERATE
    OUTPUT "${CPPTEST_TEST_DISCOVERY_FILE}"
    CONTENT
  "if(NOT EXISTS \"${CPPTEST_TESTS_FILE}\" OR
    NOT \"${CPPTEST_TESTS_FILE}\" IS_NEWER_THAN \"$<TARGET_FILE:${TARGET}>\" OR
    NOT \"${CPPTEST_TESTS_FILE}\" IS_NEWER_THAN \"\${CMAKE_CURRENT_LIST_FILE}\")
    include(\"${CPPTEST_DISCOVER_TESTS_SCRIPT}\")
    cpptest_discover_tests_script(\"$<TARGET_FILE:${TARGET}>\" \"${CPPTEST_TESTS_FILE}\"
      ${CPPTEST_TEST_DISCOVERY_EXTRA_ARGS})
  endif()
  include(\"${CPPTEST_TESTS_FILE}\")"
    TARGET ${TARGET})
endfunction()

set(CPPTEST_DISCOVER_TESTS_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/CppTestDiscoverTests.cmake )
