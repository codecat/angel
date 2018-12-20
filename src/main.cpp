#include <cstdio>

#include <string>

#include <common/version.h>
#include <common/Module.h>

#include <modules/love/love.h>

#include <angelscript.h>

#include <scriptbuilder/scriptbuilder.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptarray/scriptarray.h>

#include <angel_modules/filesystem.h>

static int g_argc;
static char* g_argv0;

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

DoneAction runangel()
{
	DoneAction done = done_Quit;

	asIScriptEngine* engine = asCreateScriptEngine();
	int r = 0;

	//int SetMessageCallback(const asSFuncPtr &callback, void *obj, asDWORD callConv)
	engine->SetMessageCallback(asFUNCTION(ScriptMessage), nullptr, asCALL_CDECL);

	RegisterScriptArray(engine, true);
	RegisterStdString(engine);
	RegisterStdStringUtils(engine);

	engine->RegisterObjectMethod("string", "string replace(const string &in search, const string &in replace) const", asFUNCTION(ScriptStringReplace), asCALL_CDECL_OBJFIRST);

	engine->RegisterGlobalFunction("void print(const string &in str)", asFUNCTION(ScriptPrint), asCALL_CDECL);

	RegisterFilesystem(engine, g_argv0);

	CScriptBuilder builder;
	builder.StartNewModule(engine, "Scripts");
	builder.AddSectionFromFile("scripts/boot.as");
	r = builder.BuildModule();
	if (r != asSUCCESS) {
		printf("Building scripts failed!\n");
		return done;
	}

	printf("%d modules:\n", love::Module::M_MAX_ENUM);
	for (int i = 0; i < love::Module::M_MAX_ENUM; i++) {
		auto mod = love::Module::getInstance<love::Module>((love::Module::ModuleType)i);
		if (mod != nullptr) {
			printf("  %s\n", mod->getName());
		}
	}

	asIScriptModule* mod = builder.GetModule();
	asIScriptFunction* func = mod->GetFunctionByDecl("void boot()");

	asIScriptContext* ctx = engine->CreateContext();
	ctx->Prepare(func);
	ctx->Execute();
	if (ctx->GetState() == asEXECUTION_EXCEPTION) {
		printf("Script exception caught!\n");
		return done;
	}
	ctx->Unprepare();

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

	return 0;
}
