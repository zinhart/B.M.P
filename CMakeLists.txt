# minimum cmake version
cmake_minimum_required(VERSION 3.1)

# library name
project(bmp)

# set build type to release by default
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type provided, so project will be build without debug flags")
  set(CMAKE_BUILD_TYPE "RELEASE")
endif()

set (BMP_INCLUDE_DIR bmp/include CACHE STRING "BMP INCLUDE DIRS")

# for unit tests using google test
option(BuildUnitTest "BuildUnitTest" ON)
# for benchmarking using google bench
option(BuildBenchmarks "BuildBenchmarks" OFF)
# library type
option(BuildShared "BuildShared" OFF)
# nesting protection
option(BmpUseCodeCoverage "BmpUseCodeCoverage" ON)
# sanitizer support
option(Sanitize "Sanitize" OFF)

#threading options 
option(PTHREADS "PTHREADS" ON)
option(WIN32_THREADS "WIN32_THREADS" OFF)

# set lib type
if(BuildShared)
  set(LIB_TYPE SHARED)
else()
  set(LIB_TYPE STATIC)
endif()

#set module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

if(PTHREADS)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
elseif(WIN32_THREADS)
  set(THREADS_PREFER_WIN32_FLAG ON)
elseif(PTHREADS AND WIN32_THREADS)
  message(FATAL_ERROR "Choose 1 threading library")
endif()
 
include_directories(. include "${BMP_INCLUDE_DIR}" "${PROJECT_BINARY_DIR}")

# set cxx standard
set(CMAKE_CXX_STANDARD 11)
# force cxx standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set cxx flags
set(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")
# set cxx release flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -fPIC")

# set cxx debug flags
if(BmpUseCodeCoverage)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -pedantic -fno-omit-frame-pointer -fno-inline-functions -fPIC --coverage")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-g -pedantic -fno-omit-frame-pointer -fno-inline-functions -fPIC")
endif()

# sanitize flags
if (Sanitize)
  set(CMAKE_CXX_FLAGS "-fsantize=address -fsanitize=undefined ${CMAKE_CXX_FLAGS}")
endif()


# add src directory
add_subdirectory(src)


#enable testing and add unit test directory
if(BuildUnitTest)
  enable_testing()
  find_package(GTest)

  if(GTEST_FOUND)
	# add include directories
	include_directories(${GTEST_INCLUDE_DIRS})
  else()
	# Download and unpack googletest at configure time
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ExternalProjects/CMakeLists.txt.in.gtest googletest-download/CMakeLists.txt)
	execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
	if(result)
	  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
	endif()
	execute_process(COMMAND ${CMAKE_COMMAND} --build .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
	if(result)
	  message(FATAL_ERROR "Build step for googletest failed: ${result}")
	endif()

	# Prevent overriding the parent project's compiler/linker
	# settings on Windows
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


	# Conditionally Add googletest directly to our build, if this is not a sub project. This defines
	# the gtest and gtest_main targets.
	string(REGEX MATCH "${CMAKE_CURRENT_SOURCE_DIR}" result "${CMAKE_SOURCE_DIR}")
	if(${result} MATCHES "${CMAKE_CURRENT_SOURCE_DIR}")
	  add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
	  			     ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
  				     EXCLUDE_FROM_ALL 
				    )
	endif()

  endif()
  add_subdirectory(unit_tests)

endif()

if(BuildBenchmarks)
  #  set(CMAKE_ARGS "${CMAKE_ARGS} -DBENCHMARK_ENABLE_TESTING${STRING}=OFF") would like to config this with cmake args
  #  set(CMAKE_CACHE_ARGS "${CMAKE_CACHE_ARGS} -DBENCHMARK_ENABLE_TESTING${STRING}=OFF")
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)

  message(STATUS "RIGHT HERE: ${CMAKE_CURRENT_BINARY_DIR}")
  set(GTEST_ROOT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/googletest-src/googlemock" CACHE PATH "Path to externally added gtest")

  if(BuildUnitTest)
	#set(GTEST_ROOT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/googletest-src/googletest" CACHE PATH "Path to externally added gtest")
	#$  list(APPEND CMAKE_ARGS "-D${GTEST_ROOT}:${string}=\"${GTEST_ROOT_LOCATION}\"")
  endif()

  # Download and unpack googlebench at configure time
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ExternalProjects/CMakeLists.txt.in.gbench googlebench-download/CMakeLists.txt)
  execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googlebench-download )
  if(result)
	message(FATAL_ERROR "CMake step for googlebench failed: ${result}")
  endif()
  execute_process(COMMAND ${CMAKE_COMMAND} --build .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googlebench-download )
  if(result)
	message(FATAL_ERROR "Build step for googlebench failed: ${result}")
  endif()


  # Add googlebench directly to our build. This defines
  # the gtest and gtest_main targets.
  string(REGEX MATCH "${CMAKE_CURRENT_SOURCE_DIR}" result "${CMAKE_SOURCE_DIR}")
  if(${result} MATCHES "${CMAKE_CURRENT_SOURCE_DIR}")
   	add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googlebench-src
					 ${CMAKE_CURRENT_BINARY_DIR}/googlebench-build
				     EXCLUDE_FROM_ALL
					)
  endif()
  add_subdirectory(benchmarks)
endif()

# code coverage
if(BmpUseCodeCoverage)
  if(CMAKE_COMPILER_IS_GNUCXX)
	  include(CodeCoverage)
	  # ignore extraneous files
	  set(COVERAGE_LCOV_EXCLUDES '/usr/include/*' '${PROJECT_BINARY_DIR}/googlebench-src/*' '${PROJECT_BINARY_DIR}/googletest-src/*')
	  SETUP_TARGET_FOR_COVERAGE_LCOV(NAME         ${PROJECT_NAME}_coverage 
									 EXECUTABLE   ./unit_tests/bmp_unit_tests 
									 DEPENDENCIES ${PROJECT_NAME})
  endif()
endif()

message(STATUS "")
message(STATUS " BUILD SUMMARY")
message(STATUS " CMAKE_GENERATOR		  : ${CMAKE_GENERATOR}")
message(STATUS " CMAKE_GENERATOR		  : ${CMAKE_GENERATOR}")
message(STATUS " Compiler ID			  : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS " Build Type			  : ${CMAKE_BUILD_TYPE}")
message(STATUS " Library Type		  : ${LIB_TYPE}")
if(BuildUnitTest)
message(STATUS " Build Tests			  : ${BuildUnitTest}")
endif()
if(BuildBenchmarks)
message(STATUS " Build Benchmarks			  : ${BuildBenchmarks}")
endif()
message(STATUS " Sanitize Flags		  : ${Sanitize}")
message(STATUS " CMAKE_CXX_FLAGS		  : ${CMAKE_CXX_FLAGS}")
if("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
	message(STATUS " CMAKE_CXX_RELEASE_FLAGS	  : ${CMAKE_CXX_FLAGS_RELEASE}")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
  message(STATUS " CMAKE_CXX_FLAGS_DEBUG         : ${CMAKE_CXX_FLAGS_DEBUG}")
endif()
message(STATUS "")
