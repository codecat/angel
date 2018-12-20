cmake_minimum_required(VERSION 3.10)

project(Angel)

add_subdirectory(../love out)
add_subdirectory(angelscript/sdk/angelscript/projects/cmake)

add_executable(angel
	src/main.cpp

	src/angel_modules/filesystem.cpp
	src/angel_modules/filesystem.h

	angelscript/sdk/add_on/scriptarray/scriptarray.cpp
	angelscript/sdk/add_on/scriptarray/scriptarray.h

	angelscript/sdk/add_on/scriptbuilder/scriptbuilder.cpp
	angelscript/sdk/add_on/scriptbuilder/scriptbuilder.h

	angelscript/sdk/add_on/scriptstdstring/scriptstdstring.cpp
	angelscript/sdk/add_on/scriptstdstring/scriptstdstring.h
	angelscript/sdk/add_on/scriptstdstring/scriptstdstring_utils.cpp
)

include_directories(
	# Our code
	src/

	# Love
	../love/src/
	../love/src/modules/

	# Temporary absolute paths
	/usr/include/SDL2/
	/usr/include/luajit-2.1/

	# Angelscript
	angelscript/sdk/angelscript/include/
	angelscript/sdk/add_on/
)

target_link_libraries(angel liblove angelscript)
