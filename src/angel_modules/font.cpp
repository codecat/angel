#include <angel_modules/font.h>

#include <modules/font/Font.h>
#include <modules/font/freetype/Font.h>

#define instance() (love::Module::getInstance<love::font::Font>(love::Module::M_FONT))

/*
static void module_newRasterizer() { instance()->newRasterizer(); }
static void module_newTrueTypeRasterizer() { instance()->newTrueTypeRasterizer(); }
static void module_newBMFontRasterizer() { instance()->newBMFontRasterizer(); }
static void module_newImageRasterizer() { instance()->newImageRasterizer(); }
static void module_newGlyphData() { instance()->newGlyphData(); }
*/

void RegisterFont(asIScriptEngine* engine)
{
	auto pFont = instance();
	if (pFont == nullptr) {
		pFont = new love::font::freetype::Font();
		love::Module::registerInstance(pFont);
	}

	engine->SetDefaultNamespace("angel::font");

	/*
	engine->RegisterGlobalFunction("void newRasterizer()", asFUNCTION(module_newRasterizer), asCALL_CDECL);
	engine->RegisterGlobalFunction("void newTrueTypeRasterizer()", asFUNCTION(module_newTrueTypeRasterizer), asCALL_CDECL);
	engine->RegisterGlobalFunction("void newBMFontRasterizer()", asFUNCTION(module_newBMFontRasterizer), asCALL_CDECL);
	engine->RegisterGlobalFunction("void newImageRasterizer()", asFUNCTION(module_newImageRasterizer), asCALL_CDECL);
	engine->RegisterGlobalFunction("void newGlyphData()", asFUNCTION(module_newGlyphData), asCALL_CDECL);
	*/

	engine->SetDefaultNamespace("");
}
