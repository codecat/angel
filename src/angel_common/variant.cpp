#include <angel_common/variant.h>
#include <angel_common/bindings.h>

#include <common/Variant.h>

static void VariantCtor_bool(love::Variant* self, bool value)
{
	new (self) love::Variant(value);
}

static void VariantCtor_double(love::Variant* self, double value)
{
	new (self) love::Variant(value);
}

static void VariantCtor_string(love::Variant* self, const std::string &value)
{
	new (self) love::Variant(value.c_str(), value.size());
}

static bool VariantGetBool(love::Variant* self)
{
	if (self->getType() != love::Variant::BOOLEAN) {
		asGetActiveContext()->SetException("Type of Variant is not Boolean!");
		return false;
	}
	return self->getData().boolean;
}

static double VariantGetNumber(love::Variant* self)
{
	if (self->getType() != love::Variant::NUMBER) {
		asGetActiveContext()->SetException("Type of Variant is not Number!");
		return 0.0;
	}
	return self->getData().number;
}

static std::string VariantGetString(love::Variant* self)
{
	if (self->getType() == love::Variant::STRING) {
		auto str = self->getData().string;
		return std::string(str->str, str->len);

	} else if (self->getType() == love::Variant::SMALLSTRING) {
		auto &str = self->getData().smallstring;
		return std::string(str.str, (size_t)str.len);

	} else {
		asGetActiveContext()->SetException("Type of Variant is not String or SmallString!");
		return "";
	}
}

void RegisterVariant(asIScriptEngine* engine)
{
	engine->SetDefaultNamespace("angel");

	// VariantType
	engine->RegisterEnum("VariantType");
	engine->RegisterEnumValue("VariantType", "Unknown", love::Variant::UNKNOWN);
	engine->RegisterEnumValue("VariantType", "Boolean", love::Variant::BOOLEAN);
	engine->RegisterEnumValue("VariantType", "Number", love::Variant::NUMBER);
	engine->RegisterEnumValue("VariantType", "String", love::Variant::STRING);
	engine->RegisterEnumValue("VariantType", "SmallString", love::Variant::SMALLSTRING);
	engine->RegisterEnumValue("VariantType", "Userdata", love::Variant::LUSERDATA);
	engine->RegisterEnumValue("VariantType", "Object", love::Variant::LOVEOBJECT);
	engine->RegisterEnumValue("VariantType", "Nil", love::Variant::NIL);
	engine->RegisterEnumValue("VariantType", "Table", love::Variant::TABLE);

	// Variant
	engine->RegisterObjectType("Variant", sizeof(love::Variant), asOBJ_VALUE | asGetTypeTraits<love::Variant>());
	engine->RegisterObjectBehaviour("Variant", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ScriptCtor<love::Variant>), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("Variant", asBEHAVE_CONSTRUCT, "void f(const Variant &in copy)", asFUNCTION(ScriptCopyCtor<love::Variant>), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("Variant", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptDtor<love::Variant>), asCALL_CDECL_OBJFIRST);

	engine->RegisterObjectBehaviour("Variant", asBEHAVE_CONSTRUCT, "void f(bool)", asFUNCTION(VariantCtor_bool), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("Variant", asBEHAVE_CONSTRUCT, "void f(double)", asFUNCTION(VariantCtor_double), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("Variant", asBEHAVE_CONSTRUCT, "void f(const string &in)", asFUNCTION(VariantCtor_string), asCALL_CDECL_OBJFIRST);

	engine->RegisterObjectMethod("Variant", "VariantType getType() const", asMETHOD(love::Variant, getType), asCALL_THISCALL);

	engine->RegisterObjectMethod("Variant", "bool getBool() const", asFUNCTION(VariantGetBool), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("Variant", "double getNumber() const", asFUNCTION(VariantGetNumber), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("Variant", "string getString() const", asFUNCTION(VariantGetString), asCALL_CDECL_OBJFIRST);

	engine->SetDefaultNamespace("");
}
