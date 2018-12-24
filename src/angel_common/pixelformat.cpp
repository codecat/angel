#include <angel_common/pixelformat.h>

#include <common/pixelformat.h>

void RegisterPixelFormat(asIScriptEngine* engine)
{
	engine->SetDefaultNamespace("angel");

	engine->RegisterEnum("PixelFormat");

	engine->SetDefaultNamespace("");
}
