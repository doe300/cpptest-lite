cmake_minimum_required (VERSION 3.1)

# Set C++ standard to C++11 without any extensions (e.g. GNU)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project (CppTest-lite VERSION 0.9)
add_definitions(-DCPPTEST_LITE_VERSION="${PROJECT_VERSION}")

option(CPPTEST_LITE_CREATE_TESTS "Creates the test-program used to test cpptest-lite" OFF)
option(ENABLE_SANITIZERS "Enable build with various sanitizers" OFF)

# For in-tree build, move libraries to build
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
endif()

#Include the header and source files
file( GLOB HDRS "include/*.h" )
file( GLOB SRCS "src/*.cpp" )

include_directories ("include/")

if(WIN32)
	#don't override flags, if set by e.g. parent-project
	if(CMAKE_CXX_FLAGS STREQUAL "")
		set(CMAKE_CXX_FLAGS "/W3 /D_UNICODE /DUNICODE")
	endif()
	add_library(cpptest-lite STATIC ${SRCS})
else()
	if(CMAKE_CXX_FLAGS STREQUAL "")
		SET(CMAKE_CXX_FLAGS " -O3 -Wall -Wextra -Wold-style-cast -Wno-unused-parameter -Wno-missing-field-initializers -pthread -fPIC -Wpedantic")
	endif()
	add_library(cpptest-lite SHARED ${SRCS})
endif()

# Enable sanitizers
if(ENABLE_SANITIZERS)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=leak -fsanitize=undefined ")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdelete-null-pointer-checks -Wnull-dereference -Wuninitialized -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=format -Wsuggest-override -Wconversion -Wzero-as-null-pointer-constant")
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
	file( GLOB TEST_SRCS "test/*.cpp")
	add_executable(testCppTestLite ${TEST_SRCS} ${HDRS})
	target_link_libraries(testCppTestLite cpptest-lite)
	
	#Add ctest targets
	enable_testing()
	add_test(NAME Failings COMMAND testCppTestLite --fail-tests WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_test(NAME Comparisons COMMAND testCppTestLite --test-comparisons WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_test(NAME Exceptions COMMAND testCppTestLite --test-exceptions WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_test(NAME Macros COMMAND testCppTestLite --test-macros WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_test(NAME Format COMMAND testCppTestLite --test-format WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_test(NAME Outputs COMMAND testCppTestLite --test-outputs WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	add_test(NAME Parallel COMMAND testCppTestLite --test-parallel WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
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
install(EXPORT cpptest-lite DESTINATION share/cpptest-lite)
install(DIRECTORY include/ DESTINATION include/cpptest-lite FILES_MATCHING PATTERN "*.h")
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "cmake_uninstall.cmake")