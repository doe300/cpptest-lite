cmake_minimum_required (VERSION 3.1)

project (CppTest-lite VERSION 0.8)
add_definitions(-DCPPTEST_LITE_VERSION="${PROJECT_VERSION}")

option(CPPTEST_LITE_CREATE_TESTS "Creates the test-program used to test cpptest-lite" ON)

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
		SET(CMAKE_CXX_FLAGS "-std=c++0x -O3 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -pthread -fPIC -Wpedantic")
	endif()
	add_library(cpptest-lite SHARED ${SRCS})
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
endif()

# Installation targets
install(TARGETS cpptest-lite EXPORT cpptest-lite LIBRARY DESTINATION lib)
install(EXPORT cpptest-lite DESTINATION share/cpptest-lite)
install(DIRECTORY include/ DESTINATION include/cpptest-lite FILES_MATCHING PATTERN "*.h")
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "cmake_uninstall.cmake")