#include <angel_common/graphics/drawable.h>

#include <class_register.h>

void RegisterGraphicsDrawable(asIScriptEngine* engine)
{
	engine->SetDefaultNamespace("angel::graphics");

	auto regDrawable = ClassRegister::New(engine, "Drawable", 0, asOBJ_REF, "Object");

	engine->SetDefaultNamespace("");
}
