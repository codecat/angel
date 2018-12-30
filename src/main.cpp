#include <common.h>

#include <common/version.h>
#include <common/Module.h>
#include <common/Object.h>

#include <modules/love/love.h>

#include <angelscript.h>

#include <scriptbuilder/scriptbuilder.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptarray/scriptarray.h>
#include <scriptdictionary/scriptdictionary.h>

#include <angel_common/object.h>
#include <angel_common/variant.h>
#include <angel_common/color.h>
#include <angel_common/pixelformat.h>
#include <angel_common/data/data.h>
#include <angel_common/math/vec2.h>
#include <angel_common/math/math.h>
#include <angel_common/graphics/drawable.h>
#include <angel_common/graphics/texture.h>
#include <angel_common/graphics/image.h>
#include <angel_common/font/rasterizer.h>

#include <angel_modules/filesystem.h>
#include <angel_modules/event.h>
#include <angel_modules/timer.h>
#include <angel_modules/window.h>
#include <angel_modules/graphics.h>
#include <angel_modules/data.h>
#include <angel_modules/image.h>
#include <angel_modules/physics.h>
#include <angel_modules/font.h>

#include <event_handlers.h>

#include <modules/filesystem/Filesystem.h>

#include <script_call.h>
#include <class_register.h>

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

asIScriptContext* g_ctx = nullptr; //TODO: MOVE THIS SOMEWHERE ELSE

static asIScriptFunction* g_funcGameLoad = nullptr;
static asIScriptFunction* g_funcGameUpdate = nullptr;
static asIScriptFunction* g_funcGameDraw = nullptr;

static std::string ScriptGetVersion()
{
	return love::VERSION;
}

static std::string ScriptGetVersionCodename()
{
	return love::VERSION_CODENAME;
}

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
	engine->SetMessageCallback(asFUNCTION(ScriptMessage), nullptr, asCALL_CDECL);

	g_ctx = engine->CreateContext();

	// Base bindings
	RegisterScriptArray(engine, true);
	RegisterStdString(engine);
	RegisterStdStringUtils(engine);
	RegisterScriptDictionary(engine);

	// Common bindings
	RegisterObject(engine);
	RegisterVariant(engine);
	RegisterColor(engine);
	RegisterPixelFormat(engine);
	RegisterDataData(engine);
	RegisterMathVec2(engine);
	RegisterMathMath(engine);
	RegisterGraphicsDrawable(engine);
	RegisterGraphicsTexture(engine);
	RegisterGraphicsImage(engine);
	RegisterFontRasterizer(engine);

	// Module bindings
	RegisterFilesystem(engine, g_argv0);
	RegisterEvent(engine);
	RegisterTimer(engine);
	RegisterWindow(engine);
	RegisterGraphics(engine);
	RegisterData(engine);
	RegisterImage(engine);
	RegisterPhysics(engine);
	RegisterFont(engine);

	engine->RegisterObjectMethod("string", "string replace(const string &in search, const string &in replace) const", asFUNCTION(ScriptStringReplace), asCALL_CDECL_OBJFIRST);

	engine->RegisterGlobalFunction("void print(const string &in str)", asFUNCTION(ScriptPrint), asCALL_CDECL);

	engine->SetDefaultNamespace("angel");

	engine->RegisterGlobalFunction("string getVersion()", asFUNCTION(ScriptGetVersion), asCALL_CDECL);
	engine->RegisterGlobalFunction("string getVersionCodename()", asFUNCTION(ScriptGetVersionCodename), asCALL_CDECL);

	engine->RegisterGlobalFunction("void game_load()", asFUNCTION(ScriptGameLoad), asCALL_CDECL);
	engine->RegisterGlobalFunction("void game_update(double dt)", asFUNCTION(ScriptGameUpdate), asCALL_CDECL);
	engine->RegisterGlobalFunction("void game_draw()", asFUNCTION(ScriptGameDraw), asCALL_CDECL);
	engine->RegisterGlobalFunction("void game_event(angel::event::Message@ msg)", asFUNCTION(EventHandlers::ScriptGameEvent), asCALL_CDECL);

	engine->SetDefaultNamespace("");

	int r = 0;

	CScriptBuilder builder;
	builder.StartNewModule(engine, "Boot");
	builder.AddSectionFromFile("scripts/boot.as");
	builder.AddSectionFromFile("scripts/graphics.as");
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

	EventHandlers::FindGameEvents(modGame);

	asIScriptFunction* funcRun = modBoot->GetFunctionByDecl("void angel_run()");
	CScriptCall(g_ctx, funcRun).Execute();

	ClassRegister::Clear();

	g_ctx->Release();
	engine->Release();

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
