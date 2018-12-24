#include <angel_common/math/vec2.h>
#include <angel_common/bindings.h>

#include <glm/vec2.hpp>

static void ScriptVec2Ctor(glm::vec2* self, float x, float y)
{
	new (self) glm::vec2(x, y);
}

void RegisterMathVec2(asIScriptEngine* engine)
{
	engine->RegisterObjectType("vec2", sizeof(glm::vec2), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<glm::vec2>());
	engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ScriptCtor<glm::vec2>), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(const vec2 &in copy)", asFUNCTION(ScriptCopyCtor<glm::vec2>), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("vec2", asBEHAVE_CONSTRUCT, "void f(float x, float y)", asFUNCTION(ScriptVec2Ctor), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("vec2", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptDtor<glm::vec2>), asCALL_CDECL_OBJFIRST);

	engine->RegisterObjectMethod("vec2", "vec2 &opAssign(const vec2 &in)", asMETHODPR(glm::vec2, operator=, (const glm::vec2 &), glm::vec2&), asCALL_THISCALL);

	engine->RegisterObjectMethod("vec2", "vec2 &opAddAssign(const vec2 &in)", asMETHODPR(glm::vec2, operator+=, (const glm::vec2 &), glm::vec2&), asCALL_THISCALL);
	engine->RegisterObjectMethod("vec2", "vec2 &opSubAssign(const vec2 &in)", asMETHODPR(glm::vec2, operator-=, (const glm::vec2 &), glm::vec2&), asCALL_THISCALL);
	engine->RegisterObjectMethod("vec2", "vec2 &opMulAssign(const vec2 &in)", asMETHODPR(glm::vec2, operator*=, (const glm::vec2 &), glm::vec2&), asCALL_THISCALL);
	engine->RegisterObjectMethod("vec2", "vec2 &opDivAssign(const vec2 &in)", asMETHODPR(glm::vec2, operator/=, (const glm::vec2 &), glm::vec2&), asCALL_THISCALL);

	engine->RegisterObjectMethod("vec2", "vec2 &opAddAssign(const float &in)", asMETHODPR(glm::vec2, operator+=, (const float &), glm::vec2&), asCALL_THISCALL);
	engine->RegisterObjectMethod("vec2", "vec2 &opSubAssign(const float &in)", asMETHODPR(glm::vec2, operator-=, (const float &), glm::vec2&), asCALL_THISCALL);
	engine->RegisterObjectMethod("vec2", "vec2 &opMulAssign(const float &in)", asMETHODPR(glm::vec2, operator*=, (const float &), glm::vec2&), asCALL_THISCALL);
	engine->RegisterObjectMethod("vec2", "vec2 &opDivAssign(const float &in)", asMETHODPR(glm::vec2, operator/=, (const float &), glm::vec2&), asCALL_THISCALL);

	engine->RegisterObjectMethod("vec2", "vec2 opSub(const vec2 &in) const", asFUNCTIONPR(glm::operator-, (const glm::vec2&, const glm::vec2&), glm::vec2), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("vec2", "vec2 opAdd(const vec2 &in) const", asFUNCTIONPR(glm::operator+, (const glm::vec2&, const glm::vec2&), glm::vec2), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("vec2", "vec2 opMul(const vec2 &in) const", asFUNCTIONPR(glm::operator*, (const glm::vec2&, const glm::vec2&), glm::vec2), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("vec2", "vec2 opDiv(const vec2 &in) const", asFUNCTIONPR(glm::operator/, (const glm::vec2&, const glm::vec2&), glm::vec2), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("vec2", "vec2 opMul(float) const", asFUNCTIONPR(glm::operator*, (const glm::vec2&, float), glm::vec2), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("vec2", "vec2 opDiv(float) const", asFUNCTIONPR(glm::operator/, (const glm::vec2&, float), glm::vec2), asCALL_CDECL_OBJFIRST);

	engine->RegisterObjectProperty("vec2", "float x", asOFFSET(glm::vec2, x));
	engine->RegisterObjectProperty("vec2", "float y", asOFFSET(glm::vec2, y));
}
