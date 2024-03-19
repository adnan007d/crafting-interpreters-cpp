cmake_minimum_required(VERSION 3.27)

project(cpplox)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB PROJECT_SOURCES  src/*.h src/*.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})


set(COMPILE_OPTIONS -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wpedantic -Wold-style-cast -Wcast-align
  -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches 
  -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wformat=2 -Wlifetime -Wimplicit-fallthrough)

# Enable AddressSanitizer, Memory & UBsan for debug mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(COMPILE_OPTIONS "${COMPILE_OPTIONS} -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=memory -fsanitize-memory-track-origins")
endif()

target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra)
