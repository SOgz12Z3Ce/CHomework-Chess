## project settings
cmake_minimum_required(VERSION 4.1.2)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
project(chess C)

## global settings
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/release")
set(SDL3_ROOT "${CMAKE_SOURCE_DIR}/third_party/SDL3-3.2.24")

## output settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIR})

## third-party include
# SDL3
include_directories("${SDL3_ROOT}/include")

## targets
add_executable(chess "${CMAKE_SOURCE_DIR}/src/main.c")

## link
target_link_libraries(chess PRIVATE "${SDL3_ROOT}/lib/libSDL3.dll.a")

## pack
# copy SDL3.dll
add_custom_command(
	TARGET chess POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL3_ROOT}/bin/SDL3.dll" $<TARGET_FILE_DIR:chess>
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL3_ROOT}/share/licenses/SDL3/LICENSE.txt" "$<TARGET_FILE_DIR:chess>/licenses/SDL3/LICENSE.txt"
)
