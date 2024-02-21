# Almost all CMake files should start with this
# You should always specify a range with the newest
# and oldest tested versions of CMake. This will ensure
# you pick up the best policies.
# cmake_minimum_required(VERSION 3.1...3.28)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(
  jpegLoader
  # VERSION 1.0
  LANGUAGES C)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(JPEG_LIBRARY "/opt/homebrew/Cellar/jpeg/9f/lib/libjpeg.a")
set(JPEG_INCLUDE_DIR "/opt/homebrew/Cellar/jpeg/9f/include/")
include_directories(/opt/homebrew/Cellar/jpeg/9f/include)

set(SOURCE_FILES
    src/main.c)

find_package (JPEG REQUIRED)
add_library(${PROJECT_NAME}.so ${SOURCE_FILES})

# Target
add_executable(${PROJECT_NAME} ${SOURCE_FILES})


add_library(libjpeg STATIC IMPORTED)
target_link_libraries(${PROJECT_NAME}.so include libjpeg)
add_dependencies(${PROJECT_NAME} libjpeg)

# enable_testing()
