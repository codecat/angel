#include <angel_modules/timer.h>

#include <modules/window/Window.h>
#include <modules/window/sdl/Window.h>

#include <angel_common/bindings.h>

#define instance() (love::Module::getInstance<love::window::Window>(love::Module::M_WINDOW))

static int module_getDisplayCount() { return instance()->getDisplayCount(); }
static std::string module_getDisplayName(int displayindex) { return instance()->getDisplayName(displayindex); }
static bool module_setMode(int width, int height, love::window::WindowSettings &settings) { return instance()->setWindow(width, height, &settings); }
static void module_getMode(int &width, int &height, love::window::WindowSettings &settings) { instance()->getWindow(width, height, settings); }
static love::window::Window::DisplayOrientation module_getDisplayOrientation(int displayindex) { return instance()->getDisplayOrientation(displayindex); }
//static void module_getFullscreenModes() { }
static bool module_setFullscreen(bool fullscreen, love::window::Window::FullscreenType fstype) { return instance()->setFullscreen(fullscreen, fstype); }
static bool module_getFullscreen()
{
	int w, h;
	love::window::WindowSettings settings;
	instance()->getWindow(w, h, settings);
	return settings.fullscreen;
}
static bool module_isOpen() { return instance()->isOpen(); }
static void module_close() { instance()->close(); }
static void module_getDesktopDimensions(int displayindex, int &width, int &height) { instance()->getDesktopDimensions(displayindex, width, height); }
static void module_setPosition(int x, int y, int displayindex) { instance()->setPosition(x, y, displayindex); }
static void module_getPosition(int &x, int &y, int &displayindex) { instance()->getPosition(x, y, displayindex); }
//static bool module_setIcon() { } //TODO: love::image::ImageData
//static void module_getIcon() { }
static void module_setVSync(int vsync) { instance()->setVSync(vsync); }
static int module_getVSync() { return instance()->getVSync(); }
static void module_setDisplaySleepEnabled(bool enable) { instance()->setDisplaySleepEnabled(enable); }
static bool module_isDisplaySleepEnabled() { return instance()->isDisplaySleepEnabled(); }
static void module_setTitle(const std::string &title) { instance()->setWindowTitle(title); }
static const std::string &module_getTitle() { return instance()->getWindowTitle(); }
static bool module_hasFocus() { return instance()->hasFocus(); }
static bool module_hasMouseFocus() { return instance()->hasMouseFocus(); }
static bool module_isVisible() { return instance()->isVisible(); }
static double module_getDPIScale() { return instance()->getDPIScale(); }
static double module_toPixels(double x) { return instance()->toPixels(x); }
static void module_toPixels2(double wx, double wy, double &px, double &py) { instance()->toPixels(wx, wy, px, py); }
static double module_fromPixels(double x) { return instance()->fromPixels(x); }
static void module_fromPixels2(double px, double py, double &wx, double &wy) { instance()->fromPixels(px, py, wx, wy); }
static void module_minimize() { instance()->minimize(); }
static void module_maximize() { instance()->maximize(); }
static void module_restore() { instance()->restore(); }
static bool module_isMaximized() { return instance()->isMaximized(); }
static bool module_isMinimized() { return instance()->isMinimized(); }
static bool module_showMessageBox(const std::string &title, const std::string &message, love::window::Window::MessageBoxType type, bool attachtowindow)
{
	return instance()->showMessageBox(title, message, type, attachtowindow);
}
static void module_requestAttention(bool continuous) { instance()->requestAttention(continuous); }

void RegisterWindow(asIScriptEngine* engine)
{
	auto pWindow = instance();
	if (pWindow == nullptr) {
		pWindow = new love::window::sdl::Window();
		love::Module::registerInstance(pWindow);
	}

	engine->SetDefaultNamespace("love::window");

	// DisplayOrientation
	engine->RegisterEnum("DisplayOrientation");
	engine->RegisterEnumValue("DisplayOrientation", "Unknown", love::window::Window::ORIENTATION_UNKNOWN);
	engine->RegisterEnumValue("DisplayOrientation", "Landscape", love::window::Window::ORIENTATION_LANDSCAPE);
	engine->RegisterEnumValue("DisplayOrientation", "LandscapeFlipped", love::window::Window::ORIENTATION_LANDSCAPE_FLIPPED);
	engine->RegisterEnumValue("DisplayOrientation", "Portrait", love::window::Window::ORIENTATION_PORTRAIT);
	engine->RegisterEnumValue("DisplayOrientation", "PortraitFlipped", love::window::Window::ORIENTATION_PORTRAIT_FLIPPED);

	// FullscreenType
	engine->RegisterEnum("FullscreenType");
	engine->RegisterEnumValue("FullscreenType", "Exclusive", love::window::Window::FULLSCREEN_EXCLUSIVE);
	engine->RegisterEnumValue("FullscreenType", "Desktop", love::window::Window::FULLSCREEN_DESKTOP);

	// MessageBoxType
	engine->RegisterEnum("MessageBoxType");
	engine->RegisterEnumValue("MessageBoxType", "Error", love::window::Window::MESSAGEBOX_ERROR);
	engine->RegisterEnumValue("MessageBoxType", "Warning", love::window::Window::MESSAGEBOX_WARNING);
	engine->RegisterEnumValue("MessageBoxType", "Info", love::window::Window::MESSAGEBOX_INFO);

	// WindowSettings
	engine->RegisterObjectType("WindowSettings", sizeof(love::window::WindowSettings), asOBJ_VALUE | asGetTypeTraits<love::window::WindowSettings>());
	engine->RegisterObjectBehaviour("WindowSettings", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ScriptCtor<love::window::WindowSettings>), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("WindowSettings", asBEHAVE_CONSTRUCT, "void f(const WindowSettings &in copy)", asFUNCTION(ScriptCopyCtor<love::window::WindowSettings>), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("WindowSettings", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptDtor<love::window::WindowSettings>), asCALL_CDECL_OBJFIRST);

	engine->RegisterObjectProperty("WindowSettings", "bool fullscreen", asOFFSET(love::window::WindowSettings, fullscreen));
	engine->RegisterObjectProperty("WindowSettings", "FullscreenType fstype", asOFFSET(love::window::WindowSettings, fstype));
	engine->RegisterObjectProperty("WindowSettings", "int vsync", asOFFSET(love::window::WindowSettings, vsync));
	engine->RegisterObjectProperty("WindowSettings", "int msaa", asOFFSET(love::window::WindowSettings, msaa));
	engine->RegisterObjectProperty("WindowSettings", "bool stencil", asOFFSET(love::window::WindowSettings, stencil));
	engine->RegisterObjectProperty("WindowSettings", "int depth", asOFFSET(love::window::WindowSettings, depth));
	engine->RegisterObjectProperty("WindowSettings", "bool resizable", asOFFSET(love::window::WindowSettings, resizable));
	engine->RegisterObjectProperty("WindowSettings", "int minwidth", asOFFSET(love::window::WindowSettings, minwidth));
	engine->RegisterObjectProperty("WindowSettings", "int minheight", asOFFSET(love::window::WindowSettings, minheight));
	engine->RegisterObjectProperty("WindowSettings", "bool borderless", asOFFSET(love::window::WindowSettings, borderless));
	engine->RegisterObjectProperty("WindowSettings", "bool centered", asOFFSET(love::window::WindowSettings, centered));
	engine->RegisterObjectProperty("WindowSettings", "int display", asOFFSET(love::window::WindowSettings, display));
	engine->RegisterObjectProperty("WindowSettings", "bool highdpi", asOFFSET(love::window::WindowSettings, highdpi));
	engine->RegisterObjectProperty("WindowSettings", "double refreshrate", asOFFSET(love::window::WindowSettings, refreshrate));
	engine->RegisterObjectProperty("WindowSettings", "bool useposition", asOFFSET(love::window::WindowSettings, useposition));
	engine->RegisterObjectProperty("WindowSettings", "int x", asOFFSET(love::window::WindowSettings, x));
	engine->RegisterObjectProperty("WindowSettings", "int y", asOFFSET(love::window::WindowSettings, y));

	// Module
	engine->RegisterGlobalFunction("int getDisplayCount()", asFUNCTION(module_getDisplayCount), asCALL_CDECL);
	engine->RegisterGlobalFunction("string getDisplayName(int displayindex)", asFUNCTION(module_getDisplayName), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool setMode(int width, int height, WindowSettings &in settings)", asFUNCTION(module_setMode), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getMode(int &out width, int &out height, WindowSettings &out settings)", asFUNCTION(module_getMode), asCALL_CDECL);
	engine->RegisterGlobalFunction("DisplayOrientation getDisplayOrientation(int displayindex)", asFUNCTION(module_getDisplayOrientation), asCALL_CDECL);
	//engine->RegisterGlobalFunction("void getFullscreenModes()", asFUNCTION(module_getFullscreenModes), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool setFullscreen(bool fullscreen, FullscreenType = love::window::FullscreenType::Desktop)", asFUNCTION(module_setFullscreen), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool getFullscreen()", asFUNCTION(module_getFullscreen), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool isOpen()", asFUNCTION(module_isOpen), asCALL_CDECL);
	engine->RegisterGlobalFunction("void close()", asFUNCTION(module_close), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getDesktopDimensions(int displayindex, int &out width, int &out height)", asFUNCTION(module_getDesktopDimensions), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setPosition(int x, int y, int displayindex)", asFUNCTION(module_setPosition), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getPosition(int &out x, int &out y, int &out displayindex)", asFUNCTION(module_getPosition), asCALL_CDECL);
	//engine->RegisterGlobalFunction("void setIcon()", asFUNCTION(module_setIcon), asCALL_CDECL);
	//engine->RegisterGlobalFunction("void getIcon()", asFUNCTION(module_getIcon), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setVSync(int vsync)", asFUNCTION(module_setVSync), asCALL_CDECL);
	engine->RegisterGlobalFunction("int getVSync()", asFUNCTION(module_getVSync), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setDisplaySleepEnabled(bool enable)", asFUNCTION(module_setDisplaySleepEnabled), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool isDisplaySleepEnabled()", asFUNCTION(module_isDisplaySleepEnabled), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setTitle(const string &in title)", asFUNCTION(module_setTitle), asCALL_CDECL);
	engine->RegisterGlobalFunction("const string &getTitle()", asFUNCTION(module_getTitle), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool hasFocus()", asFUNCTION(module_hasFocus), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool hasMouseFocus()", asFUNCTION(module_hasMouseFocus), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool isVisible()", asFUNCTION(module_isVisible), asCALL_CDECL);
	engine->RegisterGlobalFunction("double getDPIScale()", asFUNCTION(module_getDPIScale), asCALL_CDECL);
	engine->RegisterGlobalFunction("double toPixels(double x)", asFUNCTION(module_toPixels), asCALL_CDECL);
	engine->RegisterGlobalFunction("void toPixels(double wx, double wy, double &out px, double &out py)", asFUNCTION(module_toPixels2), asCALL_CDECL);
	engine->RegisterGlobalFunction("double fromPixels(double x)", asFUNCTION(module_fromPixels), asCALL_CDECL);
	engine->RegisterGlobalFunction("void fromPixels(double px, double py, double &out wx, double &out wy)", asFUNCTION(module_fromPixels2), asCALL_CDECL);
	engine->RegisterGlobalFunction("void minimize()", asFUNCTION(module_minimize), asCALL_CDECL);
	engine->RegisterGlobalFunction("void maximize()", asFUNCTION(module_maximize), asCALL_CDECL);
	engine->RegisterGlobalFunction("void restore()", asFUNCTION(module_restore), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool isMaximized()", asFUNCTION(module_isMaximized), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool isMinimized()", asFUNCTION(module_isMinimized), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool showMessageBox(const string &in title, const string &in message, MessageBoxType type = love::window::MessageBoxType::Info, bool attachtowindow = true)", asFUNCTION(module_showMessageBox), asCALL_CDECL);
	engine->RegisterGlobalFunction("void requestAttention(bool continuous)", asFUNCTION(module_requestAttention), asCALL_CDECL);

	engine->SetDefaultNamespace("");
}
