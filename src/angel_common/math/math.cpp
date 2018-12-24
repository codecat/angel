#include <angel_common/math/math.h>

#include <cmath>
#include <ctime>
#include <cstdlib>

template<typename T> static T ScriptMin(T a, T b) { if (a < b) { return a; } return b; }
template<typename T> static T ScriptMax(T a, T b) { if (a > b) { return a; } return b; }

static int ScriptRandi(int max) { return rand() % max; }
static int ScriptRandi2(int min, int max) { return min + (rand() % (max - min)); }
static float ScriptRandf() { return (float)(rand() / (double)RAND_MAX); }

static float ScriptFloorf(float f) { return floorf(f); }
static float ScriptCeilf(float f) { return ceilf(f); }

void RegisterMathMath(asIScriptEngine* engine)
{
	srand(time(nullptr));

	engine->RegisterGlobalFunction("int min(int a, int b)", asFUNCTION(ScriptMin<int>), asCALL_CDECL);
	engine->RegisterGlobalFunction("float min(float a, float b)", asFUNCTION(ScriptMin<float>), asCALL_CDECL);

	engine->RegisterGlobalFunction("int max(int a, int b)", asFUNCTION(ScriptMax<int>), asCALL_CDECL);
	engine->RegisterGlobalFunction("float max(float a, float b)", asFUNCTION(ScriptMax<float>), asCALL_CDECL);

	engine->RegisterGlobalFunction("int randi(int max)", asFUNCTION(ScriptRandi), asCALL_CDECL);
	engine->RegisterGlobalFunction("int randi(int min, int max)", asFUNCTION(ScriptRandi2), asCALL_CDECL);
	engine->RegisterGlobalFunction("float randf()", asFUNCTION(ScriptRandf), asCALL_CDECL);

	engine->RegisterGlobalFunction("float floor(float f)", asFUNCTION(ScriptFloorf), asCALL_CDECL);
	engine->RegisterGlobalFunction("float ceil(float f)", asFUNCTION(ScriptCeilf), asCALL_CDECL);
}
