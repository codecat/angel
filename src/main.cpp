#include <common.h>

#include <common/version.h>
#include <common/Module.h>

#include <modules/love/love.h>

#include <angelscript.h>

#include <scriptbuilder/scriptbuilder.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptarray/scriptarray.h>

#include <angel_common/variant.h>

#include <angel_modules/filesystem.h>
#include <angel_modules/event.h>
#include <angel_modules/timer.h>
#include <angel_modules/window.h>

#include <modules/filesystem/Filesystem.h>
#include <modules/event/Event.h>
#include <modules/timer/Timer.h>

static int g_argc;
static char* g_argv0;

static int g_retval = 0;

enum DoneAction
{
	done_Restart,
	done_Quit,
};

class CScriptCall
{
public:
	asIScriptContext* m_ctx;
	asIScriptFunction* m_func;

public:
	CScriptCall(asIScriptContext* ctx, asIScriptFunction* func)
	{
		m_ctx = ctx;
		m_func = func;

		if (m_func != nullptr) {
			if (m_ctx->GetState() == asEXECUTION_ACTIVE) {
				m_ctx->PushState();
			}
			m_ctx->Prepare(m_func);
		}
	}

	void SetArg(int index, int num) { if (m_func == nullptr) { return; } m_ctx->SetArgDWord(index, num); }
	void SetArg(int index, double num) { if (m_func == nullptr) { return; } m_ctx->SetArgDouble(index, num); }
	void SetArg(int index, void* obj) { if (m_func == nullptr) { return; } m_ctx->SetArgObject(index, obj); }

	bool Execute()
	{
		if (m_func == nullptr) {
			return false;
		}

		m_ctx->Execute();
		if (m_ctx->GetState() == asEXECUTION_EXCEPTION) {
			printf("| Script exception: %s\n", m_ctx->GetExceptionString());
			for (asUINT i = 0; i < m_ctx->GetCallstackSize(); i++) {
				asIScriptFunction* frameFunc = m_ctx->GetFunction(i);
				int frameLine = m_ctx->GetLineNumber(i);
				printf("|   %s (Line %d)\n", frameFunc->GetDeclaration(true, true), frameLine);
			}
		}
		if (m_ctx->IsNested()) {
			m_ctx->PopState();
		} else {
			m_ctx->Unprepare();
		}
		return true;
	}
};

static void ScriptPrint(const std::string &str)
{
	printf("%s\n", str.c_str());
}

static void ScriptMessage(const asSMessageInfo* msg, void* param)
{
	const char* type = "";
	switch (msg->type) {
		case asMSGTYPE_ERROR: type = "ERROR"; break;
		case asMSGTYPE_WARNING: type = "WARN"; break;
		case asMSGTYPE_INFORMATION: type = "INFO"; break;
	}
	printf("| %s | Line %d | %s | %s\n", msg->section, msg->row, type, msg->message);
}

static std::string ScriptStringReplace(std::string* self, const std::string &search, const std::string &replace)
{
	std::string ret = *self;
	size_t startPos = 0;
	while ((startPos = ret.find(search, startPos)) != std::string::npos) {
		ret.replace(startPos, search.length(), replace);
		startPos += replace.length();
	}
	return ret;
}

static void loadscripts(CScriptBuilder &builder, const std::string &path, love::filesystem::Filesystem* pfs)
{
	std::vector<std::string> files;
	pfs->getDirectoryItems(path.c_str(), files);

	for (auto &file : files) {
		std::string relPath = file;
		if (path != "/") {
			relPath = path + "/" + file;
		}

		love::filesystem::Filesystem::Info info;
		pfs->getInfo(relPath.c_str(), info);
		if (info.type == love::filesystem::Filesystem::FILETYPE_DIRECTORY) {
			loadscripts(builder, relPath, pfs);
		} else {
			auto fileData = pfs->read(relPath.c_str());
			builder.AddSectionFromMemory(relPath.c_str(), (const char*)fileData->getData(), fileData->getSize());
			fileData->release();
		}
	}
}

static asIScriptContext* g_ctx = nullptr;
static asIScriptFunction* g_funcGameLoad = nullptr;
static asIScriptFunction* g_funcGameUpdate = nullptr;
static asIScriptFunction* g_funcGameDraw = nullptr;

static void ScriptGameLoad()
{
	CScriptCall(g_ctx, g_funcGameLoad).Execute();
}

static void ScriptGameUpdate(double dt)
{
	CScriptCall call(g_ctx, g_funcGameUpdate);
	call.SetArg(0, dt);
	call.Execute();
}

static void ScriptGameDraw()
{
	CScriptCall(g_ctx, g_funcGameDraw).Execute();
}

static DoneAction runangel()
{
	DoneAction done = done_Quit;

	asIScriptEngine* engine = asCreateScriptEngine();
	g_ctx = engine->CreateContext();

	int r = 0;

	//int SetMessageCallback(const asSFuncPtr &callback, void *obj, asDWORD callConv)
	engine->SetMessageCallback(asFUNCTION(ScriptMessage), nullptr, asCALL_CDECL);

	RegisterScriptArray(engine, true);
	RegisterStdString(engine);
	RegisterStdStringUtils(engine);

	engine->RegisterObjectMethod("string", "string replace(const string &in search, const string &in replace) const", asFUNCTION(ScriptStringReplace), asCALL_CDECL_OBJFIRST);

	engine->RegisterGlobalFunction("void print(const string &in str)", asFUNCTION(ScriptPrint), asCALL_CDECL);

	engine->SetDefaultNamespace("angel");
	engine->RegisterGlobalFunction("void game_load()", asFUNCTION(ScriptGameLoad), asCALL_CDECL);
	engine->RegisterGlobalFunction("void game_update(double dt)", asFUNCTION(ScriptGameUpdate), asCALL_CDECL);
	engine->RegisterGlobalFunction("void game_draw()", asFUNCTION(ScriptGameDraw), asCALL_CDECL);
	engine->SetDefaultNamespace("");

	RegisterVariant(engine);
	RegisterFilesystem(engine, g_argv0);
	RegisterEvent(engine);
	RegisterTimer(engine);
	RegisterWindow(engine);

	CScriptBuilder builder;
	builder.StartNewModule(engine, "Boot");
	builder.AddSectionFromFile("scripts/boot.as");
	r = builder.BuildModule();
	if (r != asSUCCESS) {
		printf("Building boot scripts failed!\n");
		return done;
	}
	asIScriptModule* modBoot = builder.GetModule();

	printf("Modules:\n");
	for (int i = 0; i < love::Module::M_MAX_ENUM; i++) {
		auto mod = love::Module::getInstance<love::Module>((love::Module::ModuleType)i);
		if (mod != nullptr) {
			printf("  %s\n", mod->getName());
		}
	}

	auto pFilesystem = love::Module::getInstance<love::filesystem::Filesystem>(love::Module::M_FILESYSTEM);
	auto pEvent = love::Module::getInstance<love::event::Event>(love::Module::M_EVENT);
	auto pTimer = love::Module::getInstance<love::timer::Timer>(love::Module::M_TIMER);

	asIScriptFunction* funcBoot = modBoot->GetFunctionByDecl("void angel_boot()");
	CScriptCall(g_ctx, funcBoot).Execute();

	builder.StartNewModule(engine, "Game");
	loadscripts(builder, "/", pFilesystem);
	r = builder.BuildModule();
	if (r != asSUCCESS) {
		printf("Building game scripts failed!\n");
		return done;
	}
	asIScriptModule* modGame = builder.GetModule();

	g_funcGameLoad = modGame->GetFunctionByDecl("void angel_load()");
	g_funcGameUpdate = modGame->GetFunctionByDecl("void angel_update(double dt)");
	g_funcGameDraw = modGame->GetFunctionByDecl("void angel_draw()");

	asIScriptFunction* funcRun = modBoot->GetFunctionByDecl("void angel_run()");
	CScriptCall(g_ctx, funcRun).Execute();

	//TODO: Move all logic below to boot.as instead (function angel_run) while keeping the game functions optional
	/*
	if (funcGameInit != nullptr) {
		g_ctx->Prepare(funcGameInit);
		g_ctx->Execute();
		if (g_ctx->GetState() == asEXECUTION_EXCEPTION) {
			printf("Script exception caught!\n");
			return done;
		}
		g_ctx->Unprepare();
	}

	if (funcGameLoad != nullptr) {
		g_ctx->Prepare(funcGameLoad);
		g_ctx->Execute();
		if (g_ctx->GetState() == asEXECUTION_EXCEPTION) {
			printf("Script exception caught!\n");
			return done;
		}
		g_ctx->Unprepare();
	}

	while (true) {
		pEvent->pump();

		love::event::Message* msg;
		while (pEvent->poll(msg)) {
			printf("  Event: \"%s\" (%d args)\n", msg->name.c_str(), (int)msg->args.size());
			if (msg->name == "quit") {
				//TODO: Allow canceling using angel_quit() function
				if (msg->args.size() > 0 && msg->args[0].getType() == love::Variant::NUMBER) {
					g_retval = (int)msg->args[0].getData().number;
				}
				return done;
			}
		}

		double dt = pTimer->step();

		if (funcGameUpdate != nullptr) {
			g_ctx->Prepare(funcGameUpdate);
			g_ctx->SetArgDouble(0, dt);
			g_ctx->Execute();
			if (g_ctx->GetState() == asEXECUTION_EXCEPTION) {
				printf("Script exception caught!\n");
				return done;
			}
			g_ctx->Unprepare();
		}

		//TODO: origin() and clear() graphics here

		if (funcGameDraw != nullptr) {
			g_ctx->Prepare(funcGameDraw);
			g_ctx->Execute();
			if (g_ctx->GetState() == asEXECUTION_EXCEPTION) {
				printf("Script exception caught!\n");
				return done;
			}
			g_ctx->Unprepare();
		}

		//TODO: present() graphics here
	}
	*/

	return done;
}

int main(int argc, char* argv[])
{
	g_argc = argc;
	g_argv0 = argv[0];

	printf("Angel %s (%s)\n", love_version(), love_codename());

	DoneAction done = done_Quit;
	do {
		done = runangel();
	} while (done != done_Quit);

	return g_retval;
}
