#include <angel_modules/graphics.h>

#include <modules/graphics/Graphics.h>
#include <modules/graphics/opengl/Graphics.h>

#include <modules/graphics/Shader.h>
#include <modules/graphics/ShaderStage.h>

#include <scriptdictionary/scriptdictionary.h>
#include <scriptarray/scriptarray.h>

#include <glm/vec2.hpp>

#include <class_register.h>

#define instance() (love::Module::getInstance<love::graphics::Graphics>(love::Module::M_GRAPHICS))

static void module_reset() { instance()->reset(); }
static void module_clear() { instance()->clear(love::graphics::OptionalColorf(), love::OptionalInt(), love::OptionalDouble()); }
static void module_clear2(const love::Colorf &color) { instance()->clear(color, love::OptionalInt(), love::OptionalDouble()); }
static void module_clear3(const love::Colorf &color, int stencil) { instance()->clear(color, stencil, love::OptionalDouble()); }
static void module_clear4(const love::Colorf &color, int stencil, double depth) { instance()->clear(color, stencil, depth); }
//static void module_discard() { instance()->discard(); }
static void module_present() { instance()->present(nullptr); }

static love::graphics::Image* module_newImage(love::Data* data)
{
	love::graphics::Image::Slices slices(love::graphics::TEXTURE_2D);

	love::graphics::Image::Settings settings;
	//TODO: configure settings (mipmaps, linear, dpiScale)

	love::image::ImageData* imgData = new love::image::ImageData(data);
	slices.set(0, 0, imgData);

	return instance()->newImage(slices, settings);
}
/*
static void module_newArrayImage() { instance()->newArrayImage(); }
static void module_newVolumeImage() { instance()->newVolumeImage(); }
static void module_newCubeImage() { instance()->newCubeImage(); }
static void module_newQuad() { instance()->newQuad(); }
static void module_newFont() { instance()->newFont(); }
static void module_newImageFont() { instance()->newImageFont(); }
static void module_newSpriteBatch() { instance()->newSpriteBatch(); }
static void module_newParticleSystem() { instance()->newParticleSystem(); }
static void module_newCanvas() { instance()->newCanvas(); }
static void module_newShader() { instance()->newShader(); }
static void module_newMesh() { instance()->newMesh(); }
static void module_newText() { instance()->newText(); }
static void module__newVideo() { instance()->_newVideo(); }

static void module_validateShader() { instance()->validateShader(); }

static void module_setCanvas() { instance()->setCanvas(); }
static void module_getCanvas() { instance()->getCanvas(); }
*/

static void module_setColor(const love::Colorf &c) { instance()->setColor(c); }
static love::Colorf module_getColor() { return instance()->getColor(); }
static void module_setBackgroundColor(const love::Colorf &c) { instance()->setBackgroundColor(c); }
static love::Colorf module_getBackgroundColor() { return instance()->getBackgroundColor(); }

/*
static void module_setNewFont() { instance()->setNewFont(); }
static void module_setFont() { instance()->setFont(); }
static void module_getFont() { instance()->getFont(); }

static void module_setColorMask() { instance()->setColorMask(); }
static void module_getColorMask() { instance()->getColorMask(); }
static void module_setBlendMode() { instance()->setBlendMode(); }
static void module_getBlendMode() { instance()->getBlendMode(); }
static void module_setDefaultFilter() { instance()->setDefaultFilter(); }
static void module_getDefaultFilter() { instance()->getDefaultFilter(); }
static void module_setDefaultMipmapFilter() { instance()->setDefaultMipmapFilter(); }
static void module_getDefaultMipmapFilter() { instance()->getDefaultMipmapFilter(); }
*/
static void module_setLineWidth(float width) { instance()->setLineWidth(width); }
/*
static void module_setLineStyle() { instance()->setLineStyle(); }
static void module_setLineJoin() { instance()->setLineJoin(); }
*/
static float module_getLineWidth() { return instance()->getLineWidth(); }
/*
static void module_getLineStyle() { instance()->getLineStyle(); }
static void module_getLineJoin() { instance()->getLineJoin(); }
static void module_setPointSize() { instance()->setPointSize(); }
static void module_getPointSize() { instance()->getPointSize(); }
static void module_setDepthMode() { instance()->setDepthMode(); }
static void module_getDepthMode() { instance()->getDepthMode(); }
static void module_setMeshCullMode() { instance()->setMeshCullMode(); }
static void module_getMeshCullMode() { instance()->getMeshCullMode(); }
static void module_setFrontFaceWinding() { instance()->setFrontFaceWinding(); }
static void module_getFrontFaceWinding() { instance()->getFrontFaceWinding(); }
static void module_setWireframe() { instance()->setWireframe(); }
static void module_isWireframe() { instance()->isWireframe(); }

static void module_setShader() { instance()->setShader(); }
static void module_getShader() { instance()->getShader(); }
*/
static void module__setDefaultShaderCode(CScriptDictionary* defaults, CScriptDictionary* defaults_gammacorrect)
{
	auto &defaultShaderCode = love::graphics::Graphics::defaultShaderCode;

	CScriptDictionary* arrays[] = {
		defaults,
		defaults_gammacorrect
	};

	asIScriptContext* ctx = asGetActiveContext();
	asIScriptEngine* engine = ctx->GetEngine();

	asITypeInfo* typeString = engine->GetTypeInfoByName("string");
	int typeIdString = typeString->GetTypeId();

	int typeIdDictionaryHandle = engine->GetTypeIdByDecl("dictionary@");

	for (int i = 0; i < 2; i++) {
		CScriptDictionary* dict = arrays[i];

		for (int lang = 0; lang < love::graphics::Shader::LANGUAGE_MAX_ENUM; lang++) {
			const char* langname;
			if (!love::graphics::Shader::getConstant((love::graphics::Shader::Language)lang, langname)) {
				continue;
			}

			CScriptDictionary* dictLang;
			if (!dict->Get(langname, &dictLang, typeIdDictionaryHandle)) {
				ctx->SetException("Missing a language or it's not the right type!");
				return;
			}

			std::string vertex;
			std::string pixel;
			std::string videopixel;
			std::string arraypixel;

			if (
				!dictLang->Get("vertex", &vertex, typeIdString) ||
				!dictLang->Get("pixel", &pixel, typeIdString) ||
				!dictLang->Get("videopixel", &videopixel, typeIdString) ||
				!dictLang->Get("arraypixel", &arraypixel, typeIdString)
				) {
				ctx->SetException("Missing a shader code entry or it's not the right type!");
				return;
			}

			defaultShaderCode[love::graphics::Shader::STANDARD_DEFAULT][lang][i].source[love::graphics::ShaderStage::STAGE_VERTEX] = vertex;
			defaultShaderCode[love::graphics::Shader::STANDARD_DEFAULT][lang][i].source[love::graphics::ShaderStage::STAGE_PIXEL] = pixel;

			defaultShaderCode[love::graphics::Shader::STANDARD_VIDEO][lang][i].source[love::graphics::ShaderStage::STAGE_VERTEX] = vertex;
			defaultShaderCode[love::graphics::Shader::STANDARD_VIDEO][lang][i].source[love::graphics::ShaderStage::STAGE_PIXEL] = videopixel;

			defaultShaderCode[love::graphics::Shader::STANDARD_ARRAY][lang][i].source[love::graphics::ShaderStage::STAGE_VERTEX] = vertex;
			defaultShaderCode[love::graphics::Shader::STANDARD_ARRAY][lang][i].source[love::graphics::ShaderStage::STAGE_PIXEL] = arraypixel;
		}
	}
}

/*
static void module_getSupported() { instance()->getSupported(); }
static void module_getCanvasFormats() { instance()->getCanvasFormats(); }
static void module_getImageFormats() { instance()->getImageFormats(); }
static void module_getRendererInfo() { instance()->getRendererInfo(); }
static void module_getSystemLimits() { instance()->getSystemLimits(); }
static void module_getTextureTypes() { instance()->getTextureTypes(); }
static void module_getStats() { instance()->getStats(); }

static void module_captureScreenshot() { instance()->captureScreenshot(); }
*/

static void module_draw(love::graphics::Drawable* drawable, const glm::vec2 &pos, float r, const glm::vec2 &scale, const glm::vec2 &origin, const glm::vec2 &shear)
{
	love::Matrix4 m(pos.x, pos.y, r, scale.x, scale.y, origin.x, origin.y, shear.x, shear.y);
	instance()->draw(drawable, m);
}

/*
static void module_drawLayer() { instance()->drawLayer(); }
static void module_drawInstanced() { instance()->drawInstanced(); }
*/

static void module_print(const std::string &s, const glm::vec2 &pos, float r, const glm::vec2 &scale, const glm::vec2 &origin, const glm::vec2 &shear)
{
	std::vector<love::graphics::Font::ColoredString> str;
	love::graphics::Font::ColoredString cs;
	cs.str = s;
	cs.color = love::Colorf(1.0f, 1.0f, 1.0f, 1.0f);
	str.emplace_back(cs);

	love::Matrix4 m(pos.x, pos.y, r, scale.x, scale.y, origin.x, origin.y, shear.x, shear.y);
	instance()->print(str, m);
}

/*
static void module_isCreated() { instance()->isCreated(); }
*/
static bool module_isActive() { return instance()->isActive(); }
/*
static void module_isGammaCorrect() { instance()->isGammaCorrect(); }
*/
static int module_getWidth() { return instance()->getWidth(); }
static int module_getHeight() { return instance()->getHeight(); }
static glm::vec2 module_getDimensions() { return glm::vec2(instance()->getWidth(), instance()->getHeight()); }
/*
static void module_getPixelWidth() { instance()->getPixelWidth(); }
static void module_getPixelHeight() { instance()->getPixelHeight(); }
static void module_getPixelDimensions() { instance()->getPixelDimensions(); }
static void module_getDPIScale() { instance()->getDPIScale(); }

static void module_setScissor() { instance()->setScissor(); }
static void module_intersectScissor() { instance()->intersectScissor(); }
static void module_getScissor() { instance()->getScissor(); }

static void module_stencil() { instance()->stencil(); }
static void module_setStencilTest() { instance()->setStencilTest(); }
static void module_getStencilTest() { instance()->getStencilTest(); }

static void module_points() { instance()->points(); }
*/
static void module_line(CScriptArray* vertices)
{
	asUINT numvertices = vertices->GetSize();
	if (numvertices == 0) {
		return;
	}

	love::Vector2* coords = instance()->getScratchBuffer<love::Vector2>(numvertices);

	for (asUINT i = 0; i < numvertices; i++) {
		auto &v = *(glm::vec2*)vertices->At(i);
		coords[i].x = v.x;
		coords[i].y = v.y;
	}

	instance()->polyline(coords, numvertices);
}
/*
static void module_rectangle() { instance()->rectangle(); }
static void module_circle() { instance()->circle(); }
static void module_ellipse() { instance()->ellipse(); }
static void module_arc() { instance()->arc(); }
static void module_polygon() { instance()->polygon(); }

static void module_flushBatch() { instance()->flushBatch(); }

static void module_getStackDepth() { instance()->getStackDepth(); }
static void module_push() { instance()->push(); }
static void module_pop() { instance()->pop(); }
static void module_rotate() { instance()->rotate(); }
static void module_scale() { instance()->scale(); }
static void module_translate() { instance()->translate(); }
static void module_shear() { instance()->shear(); }
*/
static void module_origin() { instance()->origin(); }
/*
static void module_applyTransform() { instance()->applyTransform(); }
static void module_replaceTransform() { instance()->replaceTransform(); }
static void module_transformPoint() { instance()->transformPoint(); }
static void module_inverseTransformPoint() { instance()->inverseTransformPoint(); }
*/

void RegisterGraphics(asIScriptEngine* engine)
{
	auto pGraphics = instance();
	if (pGraphics == nullptr) {
		pGraphics = new love::graphics::opengl::Graphics();
		love::Module::registerInstance(pGraphics);
	}

	engine->SetDefaultNamespace("angel::graphics");

	// Module
	engine->RegisterGlobalFunction("void reset()", asFUNCTION(module_reset), asCALL_CDECL);
	engine->RegisterGlobalFunction("void clear()", asFUNCTION(module_clear), asCALL_CDECL);
	engine->RegisterGlobalFunction("void clear(const Colorf &in color)", asFUNCTION(module_clear2), asCALL_CDECL);
	engine->RegisterGlobalFunction("void clear(const Colorf &in color, int stencil)", asFUNCTION(module_clear3), asCALL_CDECL);
	engine->RegisterGlobalFunction("void clear(const Colorf &in color, int stencil, double depth)", asFUNCTION(module_clear4), asCALL_CDECL);
	//engine->RegisterGlobalFunction("void discard()", asFUNCTION(module_discard), asCALL_CDECL);
	engine->RegisterGlobalFunction("void present()", asFUNCTION(module_present), asCALL_CDECL);

	engine->RegisterGlobalFunction("Image@ newImage(Data@ data)", asFUNCTION(module_newImage), asCALL_CDECL);
	/*
	engine->RegisterGlobalFunction("void newArrayImage()", asFUNCTION(module_newArrayImage), asCALL_CDECL);
	engine->RegisterGlobalFunction("void newVolumeImage()", asFUNCTION(module_newVolumeImage), asCALL_CDECL);
	engine->RegisterGlobalFunction("void newCubeImage()", asFUNCTION(module_newCubeImage), asCALL_CDECL);
	engine->RegisterGlobalFunction("void newQuad()", asFUNCTION(module_newQuad), asCALL_CDECL);
	engine->RegisterGlobalFunction("void newFont()", asFUNCTION(module_newFont), asCALL_CDECL);
	engine->RegisterGlobalFunction("void newImageFont()", asFUNCTION(module_newImageFont), asCALL_CDECL);
	engine->RegisterGlobalFunction("void newSpriteBatch()", asFUNCTION(module_newSpriteBatch), asCALL_CDECL);
	engine->RegisterGlobalFunction("void newParticleSystem()", asFUNCTION(module_newParticleSystem), asCALL_CDECL);
	engine->RegisterGlobalFunction("void newCanvas()", asFUNCTION(module_newCanvas), asCALL_CDECL);
	engine->RegisterGlobalFunction("void newShader()", asFUNCTION(module_newShader), asCALL_CDECL);
	engine->RegisterGlobalFunction("void newMesh()", asFUNCTION(module_newMesh), asCALL_CDECL);
	engine->RegisterGlobalFunction("void newText()", asFUNCTION(module_newText), asCALL_CDECL);
	engine->RegisterGlobalFunction("void _newVideo()", asFUNCTION(module__newVideo), asCALL_CDECL);

	engine->RegisterGlobalFunction("void validateShader()", asFUNCTION(module_validateShader), asCALL_CDECL);

	engine->RegisterGlobalFunction("void setCanvas()", asFUNCTION(module_setCanvas), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getCanvas()", asFUNCTION(module_getCanvas), asCALL_CDECL);
	*/

	engine->RegisterGlobalFunction("void setColor(const Colorf &in c)", asFUNCTION(module_setColor), asCALL_CDECL);
	engine->RegisterGlobalFunction("Colorf getColor()", asFUNCTION(module_getColor), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setBackgroundColor(const Colorf &in c)", asFUNCTION(module_setBackgroundColor), asCALL_CDECL);
	engine->RegisterGlobalFunction("Colorf getBackgroundColor()", asFUNCTION(module_getBackgroundColor), asCALL_CDECL);

	/*
	engine->RegisterGlobalFunction("void setNewFont()", asFUNCTION(module_setNewFont), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setFont()", asFUNCTION(module_setFont), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getFont()", asFUNCTION(module_getFont), asCALL_CDECL);

	engine->RegisterGlobalFunction("void setColorMask()", asFUNCTION(module_setColorMask), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getColorMask()", asFUNCTION(module_getColorMask), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setBlendMode()", asFUNCTION(module_setBlendMode), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getBlendMode()", asFUNCTION(module_getBlendMode), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setDefaultFilter()", asFUNCTION(module_setDefaultFilter), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getDefaultFilter()", asFUNCTION(module_getDefaultFilter), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setDefaultMipmapFilter()", asFUNCTION(module_setDefaultMipmapFilter), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getDefaultMipmapFilter()", asFUNCTION(module_getDefaultMipmapFilter), asCALL_CDECL);
	*/
	engine->RegisterGlobalFunction("void setLineWidth(float width)", asFUNCTION(module_setLineWidth), asCALL_CDECL);
	/*
	engine->RegisterGlobalFunction("void setLineStyle()", asFUNCTION(module_setLineStyle), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setLineJoin()", asFUNCTION(module_setLineJoin), asCALL_CDECL);
	*/
	engine->RegisterGlobalFunction("float getLineWidth()", asFUNCTION(module_getLineWidth), asCALL_CDECL);
	/*
	engine->RegisterGlobalFunction("void getLineStyle()", asFUNCTION(module_getLineStyle), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getLineJoin()", asFUNCTION(module_getLineJoin), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setPointSize()", asFUNCTION(module_setPointSize), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getPointSize()", asFUNCTION(module_getPointSize), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setDepthMode()", asFUNCTION(module_setDepthMode), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getDepthMode()", asFUNCTION(module_getDepthMode), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setMeshCullMode()", asFUNCTION(module_setMeshCullMode), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getMeshCullMode()", asFUNCTION(module_getMeshCullMode), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setFrontFaceWinding()", asFUNCTION(module_setFrontFaceWinding), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getFrontFaceWinding()", asFUNCTION(module_getFrontFaceWinding), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setWireframe()", asFUNCTION(module_setWireframe), asCALL_CDECL);
	engine->RegisterGlobalFunction("void isWireframe()", asFUNCTION(module_isWireframe), asCALL_CDECL);

	engine->RegisterGlobalFunction("void setShader()", asFUNCTION(module_setShader), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getShader()", asFUNCTION(module_getShader), asCALL_CDECL);
	*/
	engine->RegisterGlobalFunction("void _setDefaultShaderCode(dictionary@ defaults, dictionary@ defaults_gammacorrect)", asFUNCTION(module__setDefaultShaderCode), asCALL_CDECL);

	/*
	engine->RegisterGlobalFunction("void getSupported()", asFUNCTION(module_getSupported), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getCanvasFormats()", asFUNCTION(module_getCanvasFormats), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getImageFormats()", asFUNCTION(module_getImageFormats), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getRendererInfo()", asFUNCTION(module_getRendererInfo), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getSystemLimits()", asFUNCTION(module_getSystemLimits), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getTextureTypes()", asFUNCTION(module_getTextureTypes), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getStats()", asFUNCTION(module_getStats), asCALL_CDECL);

	engine->RegisterGlobalFunction("void captureScreenshot()", asFUNCTION(module_captureScreenshot), asCALL_CDECL);
	*/

	engine->RegisterGlobalFunction("void draw(Drawable@ drawable, const vec2 &in pos = vec2(), float r = 0.0f, const vec2 &in scale = vec2(1.0f, 1.0f), const vec2 &in origin = vec2(), const vec2 &in shear = vec2())", asFUNCTION(module_draw), asCALL_CDECL);

	/*
	engine->RegisterGlobalFunction("void drawLayer()", asFUNCTION(module_drawLayer), asCALL_CDECL);
	engine->RegisterGlobalFunction("void drawInstanced()", asFUNCTION(module_drawInstanced), asCALL_CDECL);
	*/

	engine->RegisterGlobalFunction("void print(const string &in s, const vec2 &in pos = vec2(), float r = 0.0f, const vec2 &in scale = vec2(1.0f, 1.0f), const vec2 &in origin = vec2(), const vec2 &in shear = vec2())", asFUNCTION(module_print), asCALL_CDECL);

	/*
	engine->RegisterGlobalFunction("void isCreated()", asFUNCTION(module_isCreated), asCALL_CDECL);
	*/
	engine->RegisterGlobalFunction("bool isActive()", asFUNCTION(module_isActive), asCALL_CDECL);
	/*
	engine->RegisterGlobalFunction("void isGammaCorrect()", asFUNCTION(module_isGammaCorrect), asCALL_CDECL);
	*/
	engine->RegisterGlobalFunction("int getWidth()", asFUNCTION(module_getWidth), asCALL_CDECL);
	engine->RegisterGlobalFunction("int getHeight()", asFUNCTION(module_getHeight), asCALL_CDECL);
	engine->RegisterGlobalFunction("vec2 getDimensions()", asFUNCTION(module_getDimensions), asCALL_CDECL);
	/*
	engine->RegisterGlobalFunction("void getPixelWidth()", asFUNCTION(module_getPixelWidth), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getPixelHeight()", asFUNCTION(module_getPixelHeight), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getPixelDimensions()", asFUNCTION(module_getPixelDimensions), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getDPIScale()", asFUNCTION(module_getDPIScale), asCALL_CDECL);

	engine->RegisterGlobalFunction("void setScissor()", asFUNCTION(module_setScissor), asCALL_CDECL);
	engine->RegisterGlobalFunction("void intersectScissor()", asFUNCTION(module_intersectScissor), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getScissor()", asFUNCTION(module_getScissor), asCALL_CDECL);

	engine->RegisterGlobalFunction("void stencil()", asFUNCTION(module_stencil), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setStencilTest()", asFUNCTION(module_setStencilTest), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getStencilTest()", asFUNCTION(module_getStencilTest), asCALL_CDECL);

	engine->RegisterGlobalFunction("void points()", asFUNCTION(module_points), asCALL_CDECL);
	*/
	engine->RegisterGlobalFunction("void line(array<vec2>@ vertices)", asFUNCTION(module_line), asCALL_CDECL);
	/*
	engine->RegisterGlobalFunction("void rectangle()", asFUNCTION(module_rectangle), asCALL_CDECL);
	engine->RegisterGlobalFunction("void circle()", asFUNCTION(module_circle), asCALL_CDECL);
	engine->RegisterGlobalFunction("void ellipse()", asFUNCTION(module_ellipse), asCALL_CDECL);
	engine->RegisterGlobalFunction("void arc()", asFUNCTION(module_arc), asCALL_CDECL);
	engine->RegisterGlobalFunction("void polygon()", asFUNCTION(module_polygon), asCALL_CDECL);

	engine->RegisterGlobalFunction("void flushBatch()", asFUNCTION(module_flushBatch), asCALL_CDECL);

	engine->RegisterGlobalFunction("void getStackDepth()", asFUNCTION(module_getStackDepth), asCALL_CDECL);
	engine->RegisterGlobalFunction("void push()", asFUNCTION(module_push), asCALL_CDECL);
	engine->RegisterGlobalFunction("void pop()", asFUNCTION(module_pop), asCALL_CDECL);
	engine->RegisterGlobalFunction("void rotate()", asFUNCTION(module_rotate), asCALL_CDECL);
	engine->RegisterGlobalFunction("void scale()", asFUNCTION(module_scale), asCALL_CDECL);
	engine->RegisterGlobalFunction("void translate()", asFUNCTION(module_translate), asCALL_CDECL);
	engine->RegisterGlobalFunction("void shear()", asFUNCTION(module_shear), asCALL_CDECL);
	*/
	engine->RegisterGlobalFunction("void origin()", asFUNCTION(module_origin), asCALL_CDECL);
	/*
	engine->RegisterGlobalFunction("void applyTransform()", asFUNCTION(module_applyTransform), asCALL_CDECL);
	engine->RegisterGlobalFunction("void replaceTransform()", asFUNCTION(module_replaceTransform), asCALL_CDECL);
	engine->RegisterGlobalFunction("void transformPoint()", asFUNCTION(module_transformPoint), asCALL_CDECL);
	engine->RegisterGlobalFunction("void inverseTransformPoint()", asFUNCTION(module_inverseTransformPoint), asCALL_CDECL);
	*/

	engine->SetDefaultNamespace("");
}
