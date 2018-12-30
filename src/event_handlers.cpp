#include <event_handlers.h>

#include <common.h>

#include <variant_helpers.h>
#include <script_call.h>

extern asIScriptContext* g_ctx; //TODO: MOVE THIS SOMEWHERE ELSE

static asIScriptFunction* findFunction(asIScriptModule* mod, const std::vector<std::string> &decls)
{
	for (auto &decl : decls) {
		auto func = mod->GetFunctionByDecl(decl.c_str());
		if (func != nullptr) {
			return func;
		}
	}
	return nullptr;
}

static asIScriptFunction* g_funcKeyPressed = nullptr;
static asIScriptFunction* g_funcKeyReleased = nullptr;
static asIScriptFunction* g_funcTextInput = nullptr;
static asIScriptFunction* g_funcTextEdited = nullptr;

static asIScriptFunction* g_funcMouseMoved = nullptr;
static asIScriptFunction* g_funcMousePressed = nullptr;
static asIScriptFunction* g_funcMouseReleased = nullptr;
static asIScriptFunction* g_funcWheelMoved = nullptr;

static asIScriptFunction* g_funcFocus = nullptr;
static asIScriptFunction* g_funcMouseFocus = nullptr;
static asIScriptFunction* g_funcVisible = nullptr;
static asIScriptFunction* g_funcResize = nullptr;

void EventHandlers::FindGameEvents(asIScriptModule* mod)
{
	g_funcKeyPressed = findFunction(mod, {
		"void angel_keypressed(string key)",
		"void angel_keypressed(string key, string scancode)",
		"void angel_keypressed(string key, string scancode, bool isrepeat)",
	});
	g_funcKeyReleased = findFunction(mod, {
		"void angel_keyreleased(string key)",
		"void angel_keyreleased(string key, string scancode)",
	});
	g_funcTextInput = findFunction(mod, {
		"void angel_textinput(string text)",
	});
	g_funcTextEdited = findFunction(mod, {
		"void angel_textedited(string text, int start, int length)",
	});

	g_funcMouseMoved = findFunction(mod, {
		"void angel_mousemoved(int x, int y)",
		"void angel_mousemoved(int x, int y, int dx, int dy)",
		"void angel_mousemoved(int x, int y, int dx, int dy, bool istouch)",
	});
	g_funcMousePressed = findFunction(mod, {
		"void angel_mousepressed(int x, int y)",
		"void angel_mousepressed(int x, int y, int button)",
		"void angel_mousepressed(int x, int y, int button, bool istouch)",
		"void angel_mousepressed(int x, int y, int button, bool istouch, int presses)",
	});
	g_funcMouseReleased = findFunction(mod, {
		"void angel_mousereleased(int x, int y)",
		"void angel_mousereleased(int x, int y, int button)",
		"void angel_mousereleased(int x, int y, int button, bool istouch)",
		"void angel_mousereleased(int x, int y, int button, bool istouch, int presses)",
	});
	g_funcWheelMoved = findFunction(mod, {
		"void angel_wheelmoved(int x, int y)",
	});

	g_funcVisible = findFunction(mod, {
		"void angel_visible(bool visible)",
	});
	g_funcFocus = findFunction(mod, {
		"void angel_focus(bool focus)",
	});
	g_funcMouseFocus = findFunction(mod, {
		"void angel_mousefocus(bool focus)",
	});
	g_funcResize = findFunction(mod, {
		"void angel_resize()",
		"void angel_resize(int w, int h)",
	});
}

void EventHandlers::ScriptGameEvent(love::event::Message* msg)
{
	if (msg->name == "keypressed") {
		std::string key = getVariantString(msg->args[0]);
		std::string scancode = getVariantString(msg->args[1]);
		bool isrepeat = getVariantBool(msg->args[2]);

		CScriptCall call(g_ctx, g_funcKeyPressed);
		call.SetArg(0, &key);
		call.SetArg(1, &scancode);
		call.SetArg(2, isrepeat);
		call.Execute();

	} else if (msg->name == "keyreleased") {
		std::string key = getVariantString(msg->args[0]);
		std::string scancode = getVariantString(msg->args[1]);

		CScriptCall call(g_ctx, g_funcKeyReleased);
		call.SetArg(0, &key);
		call.SetArg(1, &scancode);
		call.Execute();

	} else if (msg->name == "textinput") {
		std::string text = getVariantString(msg->args[0]);

		CScriptCall call(g_ctx, g_funcTextInput);
		call.SetArg(0, &text);
		call.Execute();

	} else if (msg->name == "textedited") {
		std::string text = getVariantString(msg->args[0]);
		int start = getVariantNumber<int>(msg->args[1]);
		int length = getVariantNumber<int>(msg->args[2]);

		CScriptCall call(g_ctx, g_funcTextEdited);
		call.SetArg(0, &text);
		call.SetArg(1, start);
		call.SetArg(2, length);
		call.Execute();

	} else if (msg->name == "mousemoved") {
		int x = getVariantNumber<int>(msg->args[0]);
		int y = getVariantNumber<int>(msg->args[1]);
		int dx = getVariantNumber<int>(msg->args[2]);
		int dy = getVariantNumber<int>(msg->args[3]);
		bool istouch = getVariantBool(msg->args[4]);

		CScriptCall call(g_ctx, g_funcMouseMoved);
		call.SetArg(0, x);
		call.SetArg(1, y);
		call.SetArg(2, dx);
		call.SetArg(3, dy);
		call.SetArg(4, istouch);
		call.Execute();

	} else if (msg->name == "mousepressed") {
		int x = getVariantNumber<int>(msg->args[0]);
		int y = getVariantNumber<int>(msg->args[1]);
		int button = getVariantNumber<int>(msg->args[2]);
		bool istouch = getVariantBool(msg->args[3]);
		int presses = getVariantNumber<int>(msg->args[4]);

		CScriptCall call(g_ctx, g_funcMousePressed);
		call.SetArg(0, x);
		call.SetArg(1, y);
		call.SetArg(2, button);
		call.SetArg(3, istouch);
		call.SetArg(4, presses);
		call.Execute();

	} else if (msg->name == "mousereleased") {
		int x = getVariantNumber<int>(msg->args[0]);
		int y = getVariantNumber<int>(msg->args[1]);
		int button = getVariantNumber<int>(msg->args[2]);
		bool istouch = getVariantBool(msg->args[3]);
		int presses = getVariantNumber<int>(msg->args[4]);

		CScriptCall call(g_ctx, g_funcMouseReleased);
		call.SetArg(0, x);
		call.SetArg(1, y);
		call.SetArg(2, button);
		call.SetArg(3, istouch);
		call.SetArg(4, presses);
		call.Execute();

	} else if (msg->name == "wheelmoved") {
		int x = getVariantNumber<int>(msg->args[0]);
		int y = getVariantNumber<int>(msg->args[1]);

		CScriptCall call(g_ctx, g_funcWheelMoved);
		call.SetArg(0, x);
		call.SetArg(1, y);
		call.Execute();

		//TODO: touchpressed
		//TODO: touchreleased
		//TODO: touchmoved
		//TODO: joystickpressed
		//TODO: joystickreleased
		//TODO: joystickaxis
		//TODO: joystickhat
		//TODO: gamepadpressed
		//TODO: gamepadreleased
		//TODO: gamepadaxis
		//TODO: joystickadded
		//TODO: joystickremoved

	} else if (msg->name == "focus") {
		bool focus = getVariantBool(msg->args[0]);

		CScriptCall call(g_ctx, g_funcFocus);
		call.SetArg(0, focus);
		call.Execute();

	} else if (msg->name == "mousefocus") {
		bool focus = getVariantBool(msg->args[0]);

		CScriptCall call(g_ctx, g_funcMouseFocus);
		call.SetArg(0, focus);
		call.Execute();

	} else if (msg->name == "visible") {
		bool visible = getVariantBool(msg->args[0]);

		CScriptCall call(g_ctx, g_funcVisible);
		call.SetArg(0, visible);
		call.Execute();

		//TODO: threaderror

	} else if (msg->name == "resize") {
		int w = getVariantNumber<int>(msg->args[0]);
		int h = getVariantNumber<int>(msg->args[1]);

		CScriptCall call(g_ctx, g_funcResize);
		call.SetArg(0, w);
		call.SetArg(0, h);
		call.Execute();

		//TODO: filedropped
		//TODO: directorydropped
		//TODO: lowmemory
		//TODO: displayrotated

	} else {
		printf("| Unhandled event: \"%s\" (%d args)\n", msg->name.c_str(), (int)msg->args.size());
	}
}
