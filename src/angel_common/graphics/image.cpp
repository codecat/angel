#include <angel_common/graphics/image.h>

#include <class_register.h>

void RegisterGraphicsImage(asIScriptEngine* engine)
{
	engine->SetDefaultNamespace("angel::graphics");

	auto regImage = ClassRegister::New(engine, "Image", 0, asOBJ_REF, "Texture");

	engine->SetDefaultNamespace("");
}
