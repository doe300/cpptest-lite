cmake_minimum_required (VERSION 2.6)
project (CppTest-lite)

option(CPPTEST_LITE_LOG_SUCCESS "Logs successfull tests" OFF)
option(CPPTEST_LITE_CREATE_TESTS "Creates the test-program used to test cpptest-lite" OFF)

if(CPPTEST_LITE_LOG_SUCCESS)
	add_definitions(-DCPPTEST_LITE_LOG_SUCCESS)
endif()

#Include the header and source files
file( GLOB HDRS "include/*.h" )
file( GLOB SRCS "src/*.cpp" )

include_directories ("include/")

if(WIN32)
	set(CMAKE_CXX_FLAGS "/W3 /D_UNICODE /DUNICODE")
	add_library(CppTestLite STATIC ${SRCS})
else()
	SET(CMAKE_CXX_FLAGS "-std=c++0x -O3 -Wall -pthread")
	add_library(cpptest-lite ${SRCS})
endif()

if(CPPTEST_LITE_CREATE_TESTS)
	file( GLOB TEST_SRCS "test/*.cpp")
	add_executable(testCppTestLite ${TEST_SRCS} ${HDRS})
	if(WIN32)
		target_link_libraries(testCppTestLite CppTestLite)
	else()
		target_link_libraries(testCppTestLite cpptest-lite)
	endif()
	set_target_properties(testCppTestLite PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
endif()
