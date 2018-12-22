#include <angel_common/data/data.h>

#include <common/Data.h>

static love::Data* Data_clone(love::Data* self) { return self->clone(); }
//static void Data_getData() { }
static uint64_t Data_getSize(love::Data* self) { return (uint64_t)self->getSize(); }

void RegisterDataData(asIScriptEngine* engine)
{
	engine->SetDefaultNamespace("angel");

	engine->RegisterObjectType("Data", 0, asOBJ_REF);
	engine->RegisterObjectBehaviour("Data", asBEHAVE_ADDREF, "void f()", asMETHOD(love::Object, retain), asCALL_THISCALL);
	engine->RegisterObjectBehaviour("Data", asBEHAVE_RELEASE, "void f()", asMETHOD(love::Object, release), asCALL_THISCALL);

	engine->RegisterObjectMethod("Data", "Data@ clone()", asFUNCTION(Data_clone), asCALL_CDECL_OBJFIRST);
	//engine->RegisterObjectMethod("Data", "void getData()", asFUNCTION(Data_getData), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("Data", "uint64 getSize()", asFUNCTION(Data_getSize), asCALL_CDECL_OBJFIRST);

	engine->SetDefaultNamespace("");
}
