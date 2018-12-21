#include <angel_modules/timer.h>

#include <modules/timer/Timer.h>

#define instance() (love::Module::getInstance<love::timer::Timer>(love::Module::M_TIMER))

static double module_step() { return instance()->step(); }
static double module_getDelta() { return instance()->getDelta(); }
static int module_getFPS() { return instance()->getFPS(); }
static double module_getAverageDelta() { return instance()->getAverageDelta(); }
static void module_sleep(double seconds) { return instance()->sleep(seconds); }
static double module_getTime() { return instance()->getTime(); }

void RegisterTimer(asIScriptEngine* engine)
{
	auto pTimer = instance();
	if (pTimer == nullptr) {
		pTimer = new love::timer::Timer();
		love::Module::registerInstance(pTimer);
	}

	engine->SetDefaultNamespace("angel::timer");

	engine->RegisterGlobalFunction("double step()", asFUNCTION(module_step), asCALL_CDECL);
	engine->RegisterGlobalFunction("double getDelta()", asFUNCTION(module_getDelta), asCALL_CDECL);
	engine->RegisterGlobalFunction("int getFPS()", asFUNCTION(module_getFPS), asCALL_CDECL);
	engine->RegisterGlobalFunction("double getAverageDelta()", asFUNCTION(module_getAverageDelta), asCALL_CDECL);
	engine->RegisterGlobalFunction("void sleep(double seconds)", asFUNCTION(module_sleep), asCALL_CDECL);
	engine->RegisterGlobalFunction("double getTime()", asFUNCTION(module_getTime), asCALL_CDECL);

	engine->SetDefaultNamespace("");
}
