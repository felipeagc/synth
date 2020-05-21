cmake_minimum_required(VERSION 3.8)
project(asdf LANGUAGES C CXX)

add_subdirectory(thirdparty/sdl2)
add_subdirectory(thirdparty/rtaudio)

add_library(
	asdf STATIC
	src/application.cpp
	)

target_compile_features(asdf PUBLIC cxx_std_11)
target_link_libraries(asdf PUBLIC SDL2 rtaudio)
target_include_directories(asdf PUBLIC include/)

add_executable(
	synth
	src/main.cpp
)

target_compile_features(synth PUBLIC cxx_std_11)
target_link_libraries(synth PUBLIC asdf)
