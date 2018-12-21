#include <common.h>

#include <common/version.h>
#include <common/Module.h>

#include <modules/love/love.h>

#include <angelscript.h>

#include <scriptbuilder/scriptbuilder.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptarray/scriptarray.h>

#include <angel_modules/filesystem.h>
#include <angel_modules/event.h>
#include <angel_modules/timer.h>

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

static DoneAction runangel()
{
	DoneAction done = done_Quit;

	asIScriptEngine* engine = asCreateScriptEngine();
	asIScriptContext* ctx = engine->CreateContext();

	int r = 0;

	//int SetMessageCallback(const asSFuncPtr &callback, void *obj, asDWORD callConv)
	engine->SetMessageCallback(asFUNCTION(ScriptMessage), nullptr, asCALL_CDECL);

	RegisterScriptArray(engine, true);
	RegisterStdString(engine);
	RegisterStdStringUtils(engine);

	engine->RegisterObjectMethod("string", "string replace(const string &in search, const string &in replace) const", asFUNCTION(ScriptStringReplace), asCALL_CDECL_OBJFIRST);

	engine->RegisterGlobalFunction("void print(const string &in str)", asFUNCTION(ScriptPrint), asCALL_CDECL);

	RegisterFilesystem(engine, g_argv0);
	RegisterEvent(engine);
	RegisterTimer(engine);

	CScriptBuilder builder;
	builder.StartNewModule(engine, "Boot");
	builder.AddSectionFromFile("scripts/boot.as");
	r = builder.BuildModule();
	if (r != asSUCCESS) {
		printf("Building boot scripts failed!\n");
		return done;
	}
	asIScriptModule* modBoot = builder.GetModule();

	printf("%d modules:\n", love::Module::M_MAX_ENUM);
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
	ctx->Prepare(funcBoot);
	ctx->Execute();
	if (ctx->GetState() == asEXECUTION_EXCEPTION) {
		printf("Script exception caught!\n");
		return done;
	}
	ctx->Unprepare();

	builder.StartNewModule(engine, "Game");
	builder.AddSectionFromFile("scripts/boot.as");
	loadscripts(builder, "/", pFilesystem);
	r = builder.BuildModule();
	if (r != asSUCCESS) {
		printf("Building game scripts failed!\n");
		return done;
	}
	asIScriptModule* modGame = builder.GetModule();

	asIScriptFunction* funcGameInit = modGame->GetFunctionByDecl("void angel_init()");
	asIScriptFunction* funcGameLoad = modGame->GetFunctionByDecl("void angel_load()");
	asIScriptFunction* funcGameUpdate = modGame->GetFunctionByDecl("void angel_update(double dt)");
	asIScriptFunction* funcGameDraw = modGame->GetFunctionByDecl("void angel_draw()");

	//TODO: Move all logic below to boot.as instead (function angel_run) while keeping the game functions optional
	if (funcGameInit != nullptr) {
		ctx->Prepare(funcGameInit);
		ctx->Execute();
		if (ctx->GetState() == asEXECUTION_EXCEPTION) {
			printf("Script exception caught!\n");
			return done;
		}
		ctx->Unprepare();
	}

	if (funcGameLoad != nullptr) {
		ctx->Prepare(funcGameLoad);
		ctx->Execute();
		if (ctx->GetState() == asEXECUTION_EXCEPTION) {
			printf("Script exception caught!\n");
			return done;
		}
		ctx->Unprepare();
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
			ctx->Prepare(funcGameUpdate);
			ctx->SetArgDouble(0, dt);
			ctx->Execute();
			if (ctx->GetState() == asEXECUTION_EXCEPTION) {
				printf("Script exception caught!\n");
				return done;
			}
			ctx->Unprepare();
		}

		//TODO: origin() and clear() graphics here

		if (funcGameDraw != nullptr) {
			ctx->Prepare(funcGameDraw);
			ctx->Execute();
			if (ctx->GetState() == asEXECUTION_EXCEPTION) {
				printf("Script exception caught!\n");
				return done;
			}
			ctx->Unprepare();
		}

		//TODO: present() graphics here
	}

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
