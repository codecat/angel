#include <angel_common/data/data.h>

#include <common/Data.h>
#include <modules/data/ByteData.h>

#include <class_register.h>

static love::Data* Data_clone(love::Data* self) { return self->clone(); }
//static void Data_getData() { }
static uint64_t Data_getSize(love::Data* self) { return (uint64_t)self->getSize(); }

static love::Data* ScriptDataStringCtor(const std::string &str)
{
	return new love::data::ByteData(str.c_str(), str.size());
}

void RegisterDataData(asIScriptEngine* engine)
{
	engine->SetDefaultNamespace("angel");

	auto regData = ClassRegister::New(engine, "Data", 0, asOBJ_REF, "Object");
	regData->Behavior(asBEHAVE_FACTORY, "Data@ f(const string &in)", asFUNCTION(ScriptDataStringCtor), asCALL_CDECL);
	regData->Method("Data@ clone()", asFUNCTION(Data_clone), asCALL_CDECL_OBJFIRST);
	regData->Method("uint64 getSize()", asFUNCTION(Data_getSize), asCALL_CDECL_OBJFIRST);

	engine->SetDefaultNamespace("");
}
