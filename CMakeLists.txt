cmake_minimum_required(VERSION 3.10)

project(Angel)

set(AS_NO_EXCEPTIONS ON)

add_subdirectory(../love out)
add_subdirectory(angelscript/sdk/angelscript/projects/cmake)

add_executable(angel
	src/main.cpp

	# Common
	src/angel_common/bindings.cpp
	src/angel_common/bindings.h

	src/angel_common/variant.cpp
	src/angel_common/variant.h

	src/angel_common/color.cpp
	src/angel_common/color.h

	# Modules
	src/angel_modules/filesystem.cpp
	src/angel_modules/filesystem.h

	src/angel_modules/event.cpp
	src/angel_modules/event.h

	src/angel_modules/timer.cpp
	src/angel_modules/timer.h

	src/angel_modules/window.cpp
	src/angel_modules/window.h

	src/angel_modules/graphics.cpp
	src/angel_modules/graphics.h

	# Angelscript add-ons
	angelscript/sdk/add_on/scriptarray/scriptarray.cpp
	angelscript/sdk/add_on/scriptarray/scriptarray.h

	angelscript/sdk/add_on/scriptdictionary/scriptdictionary.cpp
	angelscript/sdk/add_on/scriptdictionary/scriptdictionary.h

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
