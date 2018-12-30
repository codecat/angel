#include <angel_common/font/rasterizer.h>

#include <modules/font/Rasterizer.h>

#include <class_register.h>

void RegisterFontRasterizer(asIScriptEngine* engine)
{
	engine->SetDefaultNamespace("angel::font");

	auto regRasterizer = ClassRegister::New(engine, "Rasterizer", 0, asOBJ_REF, "Object");

	engine->SetDefaultNamespace("");
}
