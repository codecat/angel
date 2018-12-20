cmake_minimum_required(VERSION 3.10)

project(Angel)

add_subdirectory(../love out)

add_executable(angel
	src/main.cpp
)

include_directories(
	src/
	../love/src/
	../love/src/modules/

	# Temporary absolute paths
	/usr/include/SDL2/
	/usr/include/luajit-2.1/
)

target_link_libraries(angel liblove)
