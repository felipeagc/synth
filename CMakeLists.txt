cmake_minimum_required(VERSION 3.8)
project(asdf LANGUAGES C CXX)

add_subdirectory(thirdparty/sdl2)
add_subdirectory(thirdparty/rtaudio)

add_executable(
	asdf 

	src/main.cpp
)

target_compile_features(asdf PUBLIC cxx_std_11)
target_link_libraries(asdf PUBLIC SDL2 rtaudio)
target_include_directories(asdf PRIVATE include/)
