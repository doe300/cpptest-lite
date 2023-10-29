cmake_minimum_required (VERSION 3.5)

function(cpptest_discover_tests_script TEST_EXECUTABLE_PATH TEST_LIST_FILE_PATH)
  cmake_parse_arguments(CPPTEST_DISCOVER "" "WORKING_DIRECTORY" "PROPERTIES;DISABLED_TESTS;FAILING_TESTS" ${ARGN})

  execute_process(
    COMMAND "${TEST_EXECUTABLE_PATH}" --list-suites
    OUTPUT_VARIABLE CPPTEST_LIST_OUTPUT
    ERROR_VARIABLE CPPTEST_LIST_ERROR
    RESULT_VARIABLE CPPTEST_LIST_RESULT
  )

  if(NOT ${CPPTEST_LIST_RESULT} EQUAL 0)
    message(FATAL_ERROR "Error running test executable ${TEST_EXECUTABLE_PATH} with error:\n${CPPTEST_LIST_ERROR}")
  endif()

  get_filename_component(TEST_EXECUTABLE_NAME ${TEST_EXECUTABLE_PATH} NAME)

  # Output is lines of <SuiteName>:<test-parameter>
  file(REMOVE "${TEST_LIST_FILE_PATH}")
  string(REPLACE "\n" ";" CPPTEST_TEST_LIST "${CPPTEST_LIST_OUTPUT}")
  foreach(test ${CPPTEST_TEST_LIST})
    string(REPLACE ":" ";" test "${test}")
    list(GET test 0 TEST_NAME)
    list(GET test 1 TEST_PARAM)
    file(APPEND "${TEST_LIST_FILE_PATH}"
      "add_test(\"${TEST_EXECUTABLE_NAME}/${TEST_NAME}\" \"${TEST_EXECUTABLE_PATH}\" \"${TEST_PARAM}\")\n"
    )
    if(CPPTEST_DISCOVER_WORKING_DIRECTORY)
      file(APPEND "${TEST_LIST_FILE_PATH}"
        "set_tests_properties(\"${TEST_EXECUTABLE_NAME}/${TEST_NAME}\" PROPERTIES WORKING_DIRECTORY \"${CPPTEST_DISCOVER_WORKING_DIRECTORY}\")\n"
      )
    endif()
    if(CPPTEST_DISCOVER_PROPERTIES)
      string(REPLACE ";" " " CPPTEST_DISCOVER_PROPERTIES "${CPPTEST_DISCOVER_PROPERTIES}")
      file(APPEND "${TEST_LIST_FILE_PATH}"
        "set_tests_properties(\"${TEST_EXECUTABLE_NAME}/${TEST_NAME}\" PROPERTIES ${CPPTEST_DISCOVER_PROPERTIES})\n"
      )
    endif()
    if (CPPTEST_DISCOVER_DISABLED_TESTS AND "${TEST_NAME}" IN_LIST CPPTEST_DISCOVER_DISABLED_TESTS)
      file(APPEND "${TEST_LIST_FILE_PATH}"
        "set_tests_properties(\"${TEST_EXECUTABLE_NAME}/${TEST_NAME}\" PROPERTIES DISABLED TRUE)\n"
      )
    endif()
    if (CPPTEST_DISCOVER_FAILING_TESTS AND "${TEST_NAME}" IN_LIST CPPTEST_DISCOVER_FAILING_TESTS)
      file(APPEND "${TEST_LIST_FILE_PATH}"
        "set_tests_properties(\"${TEST_EXECUTABLE_NAME}/${TEST_NAME}\" PROPERTIES WILL_FAIL TRUE)\n"
      )
    endif()
    file(APPEND "${TEST_LIST_FILE_PATH}" "\n")
  endforeach()
endfunction()
