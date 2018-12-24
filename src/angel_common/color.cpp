#include <angel_common/color.h>
#include <angel_common/bindings.h>

#include <common/Color.h>

template<typename T>
static void ScriptColorCtor(love::ColorT<T>* self, T r, T g, T b, T a)
{
	new (self) love::ColorT<T>(r, g, b, a);
}

static void ScriptToColorCtor(love::Color* self, const love::Colorf &other)
{
	new (self) love::Color(love::toColor(other));
}

static void ScriptToColorfCtor(love::Colorf* self, const love::Color &other)
{
	new (self) love::Colorf(love::toColorf(other));
}

void RegisterColor(asIScriptEngine* engine)
{
	engine->SetDefaultNamespace("angel");

	// Color
	engine->RegisterObjectType("Color", sizeof(love::Color), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<love::Color>());
	engine->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ScriptCtor<love::Color>), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void f(const Color &in copy)", asFUNCTION(ScriptCopyCtor<love::Color>), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void f(uint8 r, uint8 g, uint8 b, uint8 a = 255)", asFUNCTION(ScriptColorCtor<unsigned char>), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("Color", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptDtor<love::Color>), asCALL_CDECL_OBJFIRST);

	engine->RegisterObjectProperty("Color", "uint8 r", asOFFSET(love::Color, r));
	engine->RegisterObjectProperty("Color", "uint8 g", asOFFSET(love::Color, g));
	engine->RegisterObjectProperty("Color", "uint8 b", asOFFSET(love::Color, b));
	engine->RegisterObjectProperty("Color", "uint8 a", asOFFSET(love::Color, a));

	// Colorf
	engine->RegisterObjectType("Colorf", sizeof(love::Colorf), asOBJ_VALUE | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<love::Colorf>());
	engine->RegisterObjectBehaviour("Colorf", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ScriptCtor<love::Colorf>), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("Colorf", asBEHAVE_CONSTRUCT, "void f(const Colorf &in copy)", asFUNCTION(ScriptCopyCtor<love::Colorf>), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("Colorf", asBEHAVE_CONSTRUCT, "void f(float r, float g, float b, float a = 1.0f)", asFUNCTION(ScriptColorCtor<float>), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("Colorf", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptDtor<love::Colorf>), asCALL_CDECL_OBJFIRST);

	engine->RegisterObjectProperty("Colorf", "float r", asOFFSET(love::Colorf, r));
	engine->RegisterObjectProperty("Colorf", "float g", asOFFSET(love::Colorf, g));
	engine->RegisterObjectProperty("Colorf", "float b", asOFFSET(love::Colorf, b));
	engine->RegisterObjectProperty("Colorf", "float a", asOFFSET(love::Colorf, a));

	// Conversion
	engine->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void f(const Colorf &in color)", asFUNCTION(ScriptToColorCtor), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("Colorf", asBEHAVE_CONSTRUCT, "void f(const Color &in color)", asFUNCTION(ScriptToColorfCtor), asCALL_CDECL_OBJFIRST);

	engine->SetDefaultNamespace("");
}
