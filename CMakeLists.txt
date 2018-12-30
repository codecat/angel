cmake_minimum_required(VERSION 3.10)

project(Angel)

set(AS_NO_EXCEPTIONS ON)

set(CMAKE_CXX_STANDARD 11)

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

	src/angel_common/font/rasterizer.cpp
	src/angel_common/font/rasterizer.h

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

	src/angel_modules/font.cpp
	src/angel_modules/font.h

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
	love2d/src/
	love2d/src/modules/
	love2d/src/libraries/

	# Angelscript
	angelscript/sdk/angelscript/include/
	angelscript/sdk/add_on/

	# Love dependencies
	${SDL2_INCLUDE_DIR}
	${FREETYPE_INCLUDE_DIR_ft2build}
)

if (LOVE_JIT)
	include_directories(${LUAJIT_INCLUDE_DIR})
else()
	include_directories(${LUA_INCLUDE_DIR})
endif()

target_link_libraries(angel liblove angelscript)

if (APPLE)
	#TODO: Temporary absolute path for GLM
	include_directories(/usr/local/Cellar/glm/0.9.9.3/include/)

	# Small fix for Love's physfs target on MacOS
	target_link_libraries(love_3p_physfs "-framework CoreFoundation")
	target_link_libraries(love_3p_physfs "-framework IOKit")
	target_link_libraries(love_3p_physfs "-framework CoreServices")
	target_link_libraries(love_3p_physfs "-framework AppKit")
endif()
