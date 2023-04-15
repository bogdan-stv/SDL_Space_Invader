Project (ppp06)
CMake_Minimum_Required (VERSION 3.1)

find_package(SDL2 REQUIRED)
set(SDL2_MIXER_INCLUDE_DIRS "D:/sdk/SDL2_mixer-2.6.3/x86_64-w64-mingw32/include")
set(SDL2_IMAGE_INCLUDE_DIRS "D:/sdk/SDL2_image-2.6.3/x86_64-w64-mingw32/include")
set(SDL2_TTF_INCLUDE_DIRS "D:/sdk/SDL2_ttf-2.20.2/x86_64-w64-mingw32/include")

add_compile_options(-Wall -Wempty-body -Werror -Wstrict-prototypes -Werror=maybe-uninitialized -Warray-bounds -g -lefence)

Include_Directories(
 	${SDL2_INCLUDE_DIRS}
 	${SDL2_MIXER_INCLUDE_DIRS}
 	${SDL2_IMAGE_INCLUDE_DIRS})

file(GLOB SOURCES   src/defs.h src/structs.h src/*.c src/*.h src/*/*.c src/*/*.h)

SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

Add_Executable (${CMAKE_PROJECT_NAME} ${SOURCES})
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)

set(SDL2_MIXER_LIBRARIES "D:/sdk/SDL2_mixer-2.6.3/x86_64-w64-mingw32/lib/libSDL2_mixer.dll.a")
set(SDL2_IMAGE_LIBRARIES "D:/sdk/SDL2_image-2.6.3/x86_64-w64-mingw32/lib/libSDL2_image.dll.a")
set(SDL2_TTF_INCLUDE_DIRS "D:/sdk/SDL2_ttf-2.20.2/x86_64-w64-mingw32/lib/libSDL2_ttf.dll.a")

Target_Link_Libraries (
 	${CMAKE_PROJECT_NAME}
 	${SDL2_LIBRARIES}
 	${SDL2_MIXER_LIBRARIES}
 	${SDL2_IMAGE_LIBRARIES}
)
