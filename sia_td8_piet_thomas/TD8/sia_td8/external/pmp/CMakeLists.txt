project(pmp-library)

cmake_minimum_required(VERSION 3.0.2)

cmake_policy(SET CMP0048 NEW)
if(${CMAKE_VERSION} VERSION_GREATER "3.11.0")
cmake_policy(SET CMP0072 OLD)
endif()

set(PMP_MAJOR_VERSION 1)
set(PMP_MINOR_VERSION 2)
set(PMP_PATCH_VERSION 1)
set(PMP_VERSION ${PMP_MAJOR_VERSION}.${PMP_MINOR_VERSION}.${PMP_PATCH_VERSION})

set(CMAKE_PROJECT_VERSION ${PMP_VERSION})
set(CMAKE_PROJECT_VERSION_MAJOR ${PMP_MAJOR_VERSION})
set(CMAKE_PROJECT_VERSION_MINOR ${PMP_MINOR_VERSION})
set(CMAKE_PROJECT_VERSION_PATCH ${PMP_PATCH_VERSION})

set(EIGEN_SOURCE_DIR "../eigen3")
include_directories(${EIGEN_SOURCE_DIR})

# setup PLY
set(RPLY_SOURCE_DIR "rply")
include_directories(${RPLY_SOURCE_DIR})
add_subdirectory(${RPLY_SOURCE_DIR})


# set default compiler flags
if((UNIX OR APPLE) AND CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra -Wno-deprecated-copy")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "7.0.0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-int-in-bool-context")
  endif()
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND NOT EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall -Wextra -Wno-unused-parameter")
endif()

if(WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_USE_MATH_DEFINES -DNOMINMAX -D_CRT_SECURE_NO_WARNINGS")
endif()

# make a release build by default
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type specified. Using 'Release' as default.")
  set(CMAKE_BUILD_TYPE "Release"
    CACHE STRING "The build type. Possible values: Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
endif()


if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
endif()

# choose size of the scalar type
if(PMP_SCALAR_TYPE EQUAL 64)
  message(STATUS "Using 64-bit scalar type")
  add_definitions(-DPMP_SCALAR_TYPE_64)
endif()

# choose size of the index type
if(PMP_INDEX_TYPE EQUAL 64)
  message(STATUS "Using 64-bit index type")
  add_definitions(-DPMP_INDEX_TYPE_64)
endif()

file(GLOB SOURCES ./*.cpp)
file(GLOB HEADERS ./*.h)

if(NOT (UNIX AND ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")))
  find_package(OpenMP)
  if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    if(UNIX OR APPLE)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse")
    endif()
    if(MSVC)
      add_definitions(/arch:SSE)
    endif(MSVC)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_INIT} -fopenmp")
  endif()
endif()

if (WIN32 OR EMSCRIPTEN)
  add_library(pmp STATIC ${SOURCES} ${HEADERS} $<TARGET_OBJECTS:rply>)
elseif(UNIX)
  add_library(pmp SHARED ${SOURCES} ${HEADERS} $<TARGET_OBJECTS:rply>)
endif()

set_target_properties(pmp PROPERTIES VERSION ${PMP_VERSION})

# check for recent cmake version
if(${CMAKE_VERSION} VERSION_GREATER "3.6.0")
  if(CLANG_TIDY_EXE AND FALSE) # disabled by default
    set_target_properties(
      pmp PROPERTIES
      CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
      )
  endif()
endif()


if(NOT EMSCRIPTEN)

  target_include_directories(pmp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
    $<INSTALL_INTERFACE:include/>)

  if((UNIX OR APPLE) AND CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(pmp PUBLIC "-std=c++11")
  endif()

  install(TARGETS pmp EXPORT pmpTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include)

  install(FILES ${HEADERS} DESTINATION include/pmp/)

endif()
