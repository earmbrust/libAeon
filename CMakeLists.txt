cmake_minimum_required (VERSION 2.8)
project (libaeon)

set (AEON_MAJOR_VERSION 0)
set (AEON_MINOR_VERSION 17)
set (AEON_PATCH_VERSION 0)
set (AEON_VERSION ${AEON_MAJOR_VERSION}.${AEON_MINOR_VERSION}.${AEON_PATCH_VERSION})

include (CheckIncludeFiles)
CHECK_INCLUDE_FILES (dlfcn.h HAVE_DLFCN_H)
CHECK_INCLUDE_FILES (inttypes.h HAVE_INTTYPES_H)
CHECK_INCLUDE_FILES (memory.h HAVE_MEMORY_H)
CHECK_INCLUDE_FILES (stdbool.h HAVE_STDBOOL_H)
CHECK_INCLUDE_FILES (stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILES (stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILES (strings.h HAVE_STRINGS_H)
CHECK_INCLUDE_FILES (string.h HAVE_string_H)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)
ADD_DEFINITIONS(-DHAVE_CONFIG_H)

include_directories(src)

add_library(aeon SHARED src/libaeon.cpp)
add_library(aeon-static STATIC src/libaeon.cpp)
set_target_properties(aeon-static PROPERTIES OUTPUT_NAME aeon)
IF (WIN32) 
	TARGET_LINK_LIBRARIES(aeon ws2_32)
ENDIF(WIN32)

add_executable(http-client example/http-client.cpp)
target_link_libraries(http-client aeon)

add_executable(hello-world example/hello-world.cpp)
target_link_libraries(hello-world aeon)

add_executable(simple-server example/simple-server.cpp)
target_link_libraries(simple-server aeon)