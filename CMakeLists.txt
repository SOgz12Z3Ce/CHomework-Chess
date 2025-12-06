# global
cmake_minimum_required(VERSION 4.1.2)
set(CMAKE_C_STANDARD 23)
project(chess C)

# paths
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(ASSETS_DIR "${PROJECT_SOURCE_DIR}/assets")
set(SDL3_DIR "${PROJECT_SOURCE_DIR}/third_party/SDL3-3.2.24")
set(SDL3_TTF_DIR "${PROJECT_SOURCE_DIR}/third_party/SDL3_ttf-3.1.0")
set(RELEASE_DIR "${CMAKE_SOURCE_DIR}/release")

# output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${RELEASE_DIR}")

# targets
add_executable(chess)
target_include_directories(chess PRIVATE
	"${SDL3_DIR}/include"
	"${SDL3_TTF_DIR}/include"
)
target_sources(chess PRIVATE
	"${SRC_DIR}/main.c"
)
target_link_libraries(chess PRIVATE
	"${SDL3_DIR}/lib/libSDL3.dll.a"
	"${SDL3_TTF_DIR}/lib/libSDL3_ttf.dll.a"
)

# copy files
add_custom_command(TARGET chess POST_BUILD
	# dlls
	COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${SDL3_DIR}/bin/SDL3.dll" "$<TARGET_FILE_DIR:chess>"
	COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${SDL3_TTF_DIR}/bin/SDL3_ttf.dll" "$<TARGET_FILE_DIR:chess>"
	
	# assets
	COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_FILE_DIR:chess>/assets"
	COMMAND "${CMAKE_COMMAND}" -E copy_directory_if_different "${ASSETS_DIR}" "$<TARGET_FILE_DIR:chess>/assets"

	# licenses
	COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_FILE_DIR:chess>/licenses"
	COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${SDL3_DIR}/share/licenses/SDL3/LICENSE.txt" "$<TARGET_FILE_DIR:chess>/licenses/SDL/LICENSE.txt"
	COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${SDL3_DIR}/share/licenses/SDL3/LICENSE.txt" "$<TARGET_FILE_DIR:chess>/licenses/SDL_ttf/LICENSE.txt"
)
