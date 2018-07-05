cmake_minimum_required (VERSION 3.1)

# Set C++ standard to C++11 without any extensions (e.g. GNU)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -rdynamic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project (CppTest-lite VERSION 0.9)
find_package(Threads REQUIRED)

option(CPPTEST_LITE_CREATE_TESTS "Creates the test-program used to test cpptest-lite" OFF)
option(ENABLE_SANITIZERS "Enable build with various sanitizers" OFF)

# For in-tree build, move libraries to build
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
endif()

if(WIN32)
	add_library(cpptest-lite STATIC "")
	#don't override flags, if set by e.g. parent-project
	if(CMAKE_CXX_FLAGS STREQUAL "")
		target_compile_options(cpptest-lite PRIVATE /W3 /D_UNICODE /DUNICODE)
	endif()
else()
	add_library(cpptest-lite SHARED "")
	if(CMAKE_CXX_FLAGS STREQUAL "")
		target_compile_options(cpptest-lite PRIVATE -O3 -Wall -Wextra -Wold-style-cast -Wno-unused-parameter -Wno-missing-field-initializers -Wpedantic -fPIC)
	endif()
endif()
target_link_libraries(cpptest-lite ${CMAKE_THREAD_LIBS_INIT})

#Include the header and source files
target_sources(cpptest-lite
  PRIVATE
    src/BDDSuite.cpp
    src/CollectorOutput.cpp
    src/CompilerOutput.cpp
    src/ConsoleOutput.cpp
    src/HTMLOutput.cpp
    src/ParallelSuite.cpp
    src/SynchronizedOutput.cpp
    src/TestSuite.cpp
    src/TextOutput.cpp
)

# Enable sanitizers
if(ENABLE_SANITIZERS)
	target_compile_options(cpptest-lite PRIVATE -fsanitize=address -fsanitize=leak -fsanitize=undefined)
	target_compile_options(cpptest-lite PRIVATE -fdelete-null-pointer-checks -Wnull-dereference -Wuninitialized -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=format -Wsuggest-override -Wconversion -Wzero-as-null-pointer-constant)
	target_link_libraries(cpptest-lite asan ubsan)
endif()

# "For shared libraries VERSION and SOVERSION can be used to specify the build version and API version respectively."
set_target_properties(
	cpptest-lite PROPERTIES
	# This corresponds to the project/library-version
	VERSION "${PROJECT_VERSION}"
	# This corresponds to the API-version (e.g. CppTest 1.1.2)
	SOVERSION "1.1.2"
)

if(CPPTEST_LITE_CREATE_TESTS)
	add_executable(testCppTestLite test/run_tests.cpp)
	target_link_libraries(testCppTestLite cpptest-lite)
	
	target_sources(testCppTestLite
	  PRIVATE
	    test/TestAssertions.cpp
	    test/TestAssertions.h
	    test/TestBDD.h
	    test/TestFormat.cpp
	    test/TestFormat.h
	    test/TestMacros.cpp
	    test/TestMacros.h
	    test/TestOutputs.cpp
	    test/TestOutputs.h
	    test/TestParallelSuite.cpp
	    test/TestParallelSuite.h
	    test/TestSuites.h
	)

	#Add ctest targets
	enable_testing()
	add_test(NAME Failings COMMAND testCppTestLite --fail-tests WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_test(NAME Comparisons COMMAND testCppTestLite --test-comparisons WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_test(NAME Exceptions COMMAND testCppTestLite --test-exceptions WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_test(NAME Macros COMMAND testCppTestLite --test-macros WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_test(NAME Format COMMAND testCppTestLite --test-format WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_test(NAME Outputs COMMAND testCppTestLite --test-outputs WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_test(NAME Parallel COMMAND testCppTestLite --test-parallel WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_test(NAME Assertions COMMAND testCppTestLite --test-assertions WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_test(NAME Story1 COMMAND testCppTestLite --story1 WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_test(NAME Story2 COMMAND testCppTestLite --story2 WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_test(NAME Story3 COMMAND testCppTestLite --story3 WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	set_tests_properties(Failings Comparisons Exceptions Macros Format Story1 PROPERTIES WILL_FAIL TRUE)
endif()

# Installation targets
if(WIN32)
	install(TARGETS cpptest-lite EXPORT cpptest-lite ARCHIVE DESTINATION lib)
else()
	install(TARGETS cpptest-lite EXPORT cpptest-lite LIBRARY DESTINATION lib)
endif()
# Adds the public headers to the target, so they are exported
target_include_directories(cpptest-lite PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>  $<INSTALL_INTERFACE:include/cpptest-lite>)
# Creates the export target (to be used by CMake to find the INSTALLED library)
install(EXPORT cpptest-lite DESTINATION share/cpptest-lite)
# Creates the install target for the headers
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION include/cpptest-lite FILES_MATCHING PATTERN "*.h")
# Exports the target (to be used by CMake to find the SOURCE library)
export(TARGETS cpptest-lite FILE cpptest-lite-exports.cmake)
# Adds custom uninstall command
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "cmake_uninstall.cmake")