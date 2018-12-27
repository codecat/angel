cmake_minimum_required(VERSION 3.10)

project(Angel)

set(AS_NO_EXCEPTIONS ON)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH "${Angel_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

if(POLICY CMP0079)
    cmake_policy(SET CMP0079 NEW)
endif()

add_subdirectory(love2d out)
add_subdirectory(angelscript/sdk/angelscript/projects/cmake)

add_executable(angel
	src/main.cpp

	src/script_call.cpp
	src/script_call.h

	src/class_register.cpp
	src/class_register.h

	src/variant_helpers.cpp
	src/variant_helpers.h

	src/event_handlers.cpp
	src/event_handlers.h

	# Common
	src/angel_common/object.cpp
	src/angel_common/object.h

	src/angel_common/bindings.cpp
	src/angel_common/bindings.h

	src/angel_common/variant.cpp
	src/angel_common/variant.h

	src/angel_common/color.cpp
	src/angel_common/color.h

	src/angel_common/pixelformat.cpp
	src/angel_common/pixelformat.h

	src/angel_common/data/data.cpp
	src/angel_common/data/data.h

	src/angel_common/math/vec2.cpp
	src/angel_common/math/vec2.h

	src/angel_common/math/math.cpp
	src/angel_common/math/math.h

	src/angel_common/graphics/drawable.cpp
	src/angel_common/graphics/drawable.h

	src/angel_common/graphics/texture.cpp
	src/angel_common/graphics/texture.h

	src/angel_common/graphics/image.cpp
	src/angel_common/graphics/image.h

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

	src/angel_modules/data.cpp
	src/angel_modules/data.h

	src/angel_modules/image.cpp
	src/angel_modules/image.h

	src/angel_modules/physics.cpp
	src/angel_modules/physics.h

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

find_package(SDL2 REQUIRED)
message("Found SDL2 path: ${SDL2_INCLUDE_DIR}")

if (APPLE)
	find_package(Lua51 REQUIRED)
	find_package(GLM REQUIRED)

	include_directories(
		${GLM_INCLUDE_DIR}
	)

	# Small fix for Love's physfs target on MacOS
	target_link_libraries(love_3p_physfs "-framework CoreFoundation")
	target_link_libraries(love_3p_physfs "-framework IOKit")
	target_link_libraries(love_3p_physfs "-framework CoreServices")
	target_link_libraries(love_3p_physfs "-framework AppKit")
else()
	find_package(LuaJIT REQUIRED)
endif()
message("Found Lua path: ${LUA_INCLUDE_DIR}")

include_directories(
	# Our code
	src/

	# Love
	love2d/src/
	love2d/src/modules/
	love2d/src/libraries/

	# Angelscript
	angelscript/sdk/angelscript/include/
	angelscript/sdk/add_on/

	# SDL2
	${SDL2_INCLUDE_DIR}

	# Lua
	${LUA_INCLUDE_DIR}
)

target_link_libraries(angel liblove angelscript)
