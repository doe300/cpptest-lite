cmake_minimum_required (VERSION 2.6)
project (CppTest-lite)

option(CPPTEST_LITE_CREATE_TESTS "Creates the test-program used to test cpptest-lite" OFF)

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
		SET(CMAKE_CXX_FLAGS "-std=c++0x -O3 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -pthread -fPIC -Wpedantic")
	endif()
	add_library(cpptest-lite SHARED ${SRCS})
endif()

# "For shared libraries VERSION and SOVERSION can be used to specify the build version and API version respectively."
set_target_properties(
	cpptest-lite PROPERTIES
	# This corresponds to the project/library-version
	VERSION "0.7"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build"
)

if(CPPTEST_LITE_CREATE_TESTS)
	file( GLOB TEST_SRCS "test/*.cpp")
	add_executable(testCppTestLite ${TEST_SRCS} ${HDRS})
	target_link_libraries(testCppTestLite cpptest-lite)
	set_target_properties(testCppTestLite PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
endif()
