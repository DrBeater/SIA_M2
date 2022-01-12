cmake_policy(SET CMP0048 NEW)

project(sia_td_laplacien CXX)

cmake_minimum_required(VERSION 3.2.0)

set (CMAKE_CXX_STANDARD 14)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "")
set(GLFW_INSTALL        OFF CACHE BOOL "")
add_subdirectory(external/glfw)

set(BUILD_SHARED_LIBS              OFF CACHE BOOL "")
set(OPTION_BUILD_TOOLS             OFF CACHE BOOL "")
set(OPTION_BUILD_EXAMPLES          OFF CACHE BOOL "")
add_subdirectory(external/glbinding)
add_subdirectory(external/SOIL)

add_subdirectory(external/pmp)

include_directories(external/glfw/include)
include_directories(external/glbinding/source/glbinding/include)
include_directories(external/glbinding/source/glbinding-aux/include)
include_directories(external/eigen3)
include_directories(external/SOIL/src)
include_directories(external)
include_directories(include)

if (APPLE)
  set(CMAKE_MACOSX_RPATH ON)
  set(GL_SILENCE_DEPRECATION ON)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  # Enable link time optimization and set the default symbol
  # visibility to hidden (very important to obtain small binaries)
  if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
    # Default symbol visibility
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
  endif()
endif()

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS src/*.h)

add_definitions(-DDATA_DIR="${PROJECT_SOURCE_DIR}/data")

add_executable(mesh_proc ${SOURCES} ${HEADERS})

target_link_libraries(mesh_proc glfw ${GLFW_LIBRARIES} glbinding SOIL pmp)
