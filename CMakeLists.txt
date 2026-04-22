cmake_minimum_required(VERSION 3.11)
project(sgd-programming-project-spring-2026 CXX) 

#what version of C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies aka raylib
# start magic in cmake
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
  endif()
endif()
#end magic

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/lib
)

#add our source file to the project
add_executable(${PROJECT_NAME} 
	"shared.h"
	src/main.cpp
	src/overworld.h
	src/overworld.cpp
	src/world0.h
	src/world0.cpp
)
target_link_libraries(${PROJECT_NAME} raylib) #we want to link raylib to our project

#optional step, copy assets (if they exist)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)