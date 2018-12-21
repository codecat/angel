#include <angel_modules/event.h>

#include <modules/event/Event.h>
#include <modules/event/sdl/Event.h>

#define instance() (love::Module::getInstance<love::event::Event>(love::Module::M_EVENT))

static void module_pump() { instance()->pump(); }
static love::event::Message* module_poll()
{
	love::event::Message* msg;
	if (!instance()->poll(msg)) {
		return nullptr;
	}
	return msg;
}
static love::event::Message* module_wait() { return instance()->wait(); }
static void module_push(love::event::Message* msg) { instance()->push(msg); }
static void module_clear() { instance()->clear(); }
static void module_quit(int exitstatus)
{
	std::vector<love::Variant> args = { love::Variant((double)exitstatus) };
	love::StrongRef<love::event::Message> m(new love::event::Message("quit", args), love::Acquire::NORETAIN);
	instance()->push(m);
}

void RegisterEvent(asIScriptEngine* engine)
{
	auto pEvent = instance();
	if (pEvent == nullptr) {
		pEvent = new love::event::sdl::Event();
		love::Module::registerInstance(pEvent);
	}

	engine->SetDefaultNamespace("event");

	engine->RegisterObjectType("Message", 0, asOBJ_REF);
	engine->RegisterObjectBehaviour("Message", asBEHAVE_ADDREF, "void f()", asMETHOD(love::Object, retain), asCALL_THISCALL);
	engine->RegisterObjectBehaviour("Message", asBEHAVE_RELEASE, "void f()", asMETHOD(love::Object, release), asCALL_THISCALL);

	engine->RegisterGlobalFunction("void pump()", asFUNCTION(module_pump), asCALL_CDECL);
	engine->RegisterGlobalFunction("Message@ poll()", asFUNCTION(module_poll), asCALL_CDECL);
	engine->RegisterGlobalFunction("Message@ wait()", asFUNCTION(module_wait), asCALL_CDECL);
	engine->RegisterGlobalFunction("void push(Message@ msg)", asFUNCTION(module_push), asCALL_CDECL);
	engine->RegisterGlobalFunction("void clear()", asFUNCTION(module_clear), asCALL_CDECL);
	engine->RegisterGlobalFunction("void quit(int exitstatus = 0)", asFUNCTION(module_quit), asCALL_CDECL);

	engine->SetDefaultNamespace("");
}
