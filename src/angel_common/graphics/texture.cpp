#include <angel_common/graphics/texture.h>

#include <modules/graphics/Texture.h>

#include <glm/vec2.hpp>

#include <angel_common/bindings.h>

#include <class_register.h>

static love::graphics::TextureType Texture_getTextureType(love::graphics::Texture* self) { return self->getTextureType(); }

static int Texture_getWidth(love::graphics::Texture* self, int mip) { return self->getWidth(mip); }
static int Texture_getHeight(love::graphics::Texture* self, int mip) { return self->getHeight(mip); }
static glm::vec2 Texture_getDimensions(love::graphics::Texture* self, int mip)
{
	glm::vec2 ret;
	ret.x = (float)self->getWidth(mip);
	ret.y = (float)self->getHeight(mip);
	return ret;
}
static int Texture_getDepth(love::graphics::Texture* self, int mip) { return self->getDepth(mip); }

static int Texture_getLayerCount(love::graphics::Texture* self) { return self->getLayerCount(); }
static int Texture_getMipmapCount(love::graphics::Texture* self) { return self->getMipmapCount(); }

static int Texture_getPixelWidth(love::graphics::Texture* self, int mip) { return self->getPixelWidth(mip); }
static int Texture_getPixelHeight(love::graphics::Texture* self, int mip) { return self->getPixelHeight(mip); }
static glm::vec2 Texture_getPixelDimensions(love::graphics::Texture* self, int mip)
{
	glm::vec2 ret;
	ret.x = (float)self->getWidth(mip);
	ret.y = (float)self->getHeight(mip);
	return ret;
}

static float Texture_getDPIScale(love::graphics::Texture* self) { return self->getDPIScale(); }

static void Texture_setFilter(love::graphics::Texture* self, const love::graphics::Texture::Filter &f) { self->setFilter(f); }
static const love::graphics::Texture::Filter &Texture_getFilter(love::graphics::Texture* self) { return self->getFilter(); }

static void Texture_setMipmapFilter(love::graphics::Texture* self, love::graphics::Texture::FilterMode mode)
{
	auto filter = self->getFilter();
	filter.mipmap = mode;
	self->setFilter(filter);
}
static love::graphics::Texture::FilterMode Texture_getMipmapFilter(love::graphics::Texture* self)
{
	return self->getFilter().mipmap;
}

static bool Texture_setWrap(love::graphics::Texture* self, const love::graphics::Texture::Wrap &w) { return self->setWrap(w); }
static const love::graphics::Texture::Wrap &Texture_getWrap(love::graphics::Texture* self) { return self->getWrap(); }

static love::PixelFormat Texture_getFormat(love::graphics::Texture* self) { return self->getPixelFormat(); }
static bool Texture_isReadable(love::graphics::Texture* self) { return self->isReadable(); }

//static CompareMode Texture_getDepthSampleMode(love::graphics::Texture* self) { return self->getDepthSampleMode(); }
//static void Texture_setDepthSampleMode(love::graphics::Texture* self) { self->setDepthSampleMode(); }

void RegisterGraphicsTexture(asIScriptEngine* engine)
{
	engine->SetDefaultNamespace("angel::graphics");

	// TextureType
	engine->RegisterEnum("TextureType");
	engine->RegisterEnumValue("TextureType", "TwoD", love::graphics::TEXTURE_2D);
	engine->RegisterEnumValue("TextureType", "Volume", love::graphics::TEXTURE_VOLUME);
	engine->RegisterEnumValue("TextureType", "TwoDArray", love::graphics::TEXTURE_2D_ARRAY);
	engine->RegisterEnumValue("TextureType", "Cube", love::graphics::TEXTURE_CUBE);

	// FilterMode
	engine->RegisterEnum("FilterMode");
	engine->RegisterEnumValue("FilterMode", "None", love::graphics::Texture::FILTER_NONE);
	engine->RegisterEnumValue("FilterMode", "Linear", love::graphics::Texture::FILTER_LINEAR);
	engine->RegisterEnumValue("FilterMode", "Nearest", love::graphics::Texture::FILTER_NEAREST);

	// TextureFilter
	engine->RegisterObjectType("TextureFilter", sizeof(love::graphics::Texture::Filter), asOBJ_VALUE | asGetTypeTraits<love::graphics::Texture::Filter>());
	engine->RegisterObjectBehaviour("TextureFilter", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ScriptCtor<love::graphics::Texture::Filter>), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("TextureFilter", asBEHAVE_CONSTRUCT, "void f(const TextureFilter &in)", asFUNCTION(ScriptCopyCtor<love::graphics::Texture::Filter>), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("TextureFilter", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptDtor<love::graphics::Texture::Filter>), asCALL_CDECL_OBJFIRST);

	engine->RegisterObjectProperty("TextureFilter", "FilterMode min", asOFFSET(love::graphics::Texture::Filter, min));
	engine->RegisterObjectProperty("TextureFilter", "FilterMode mag", asOFFSET(love::graphics::Texture::Filter, mag));
	engine->RegisterObjectProperty("TextureFilter", "FilterMode mipmap", asOFFSET(love::graphics::Texture::Filter, mipmap));
	engine->RegisterObjectProperty("TextureFilter", "float anisotropy", asOFFSET(love::graphics::Texture::Filter, anisotropy));

	// WrapMode
	engine->RegisterEnum("WrapMode");
	engine->RegisterEnumValue("WrapMode", "Clamp", love::graphics::Texture::WRAP_CLAMP);
	engine->RegisterEnumValue("WrapMode", "ClampZero", love::graphics::Texture::WRAP_CLAMP_ZERO);
	engine->RegisterEnumValue("WrapMode", "Repeat", love::graphics::Texture::WRAP_REPEAT);
	engine->RegisterEnumValue("WrapMode", "MirroredRepeat", love::graphics::Texture::WRAP_MIRRORED_REPEAT);

	// TextureWrap
	engine->RegisterObjectType("TextureWrap", sizeof(love::graphics::Texture::Wrap), asOBJ_VALUE | asGetTypeTraits<love::graphics::Texture::Wrap>());
	engine->RegisterObjectBehaviour("TextureWrap", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ScriptCtor<love::graphics::Texture::Wrap>), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("TextureWrap", asBEHAVE_CONSTRUCT, "void f(const TextureWrap &in)", asFUNCTION(ScriptCopyCtor<love::graphics::Texture::Wrap>), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour("TextureWrap", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptDtor<love::graphics::Texture::Wrap>), asCALL_CDECL_OBJFIRST);

	engine->RegisterObjectProperty("TextureWrap", "WrapMode s", asOFFSET(love::graphics::Texture::Wrap, s));
	engine->RegisterObjectProperty("TextureWrap", "WrapMode t", asOFFSET(love::graphics::Texture::Wrap, t));
	engine->RegisterObjectProperty("TextureWrap", "WrapMode r", asOFFSET(love::graphics::Texture::Wrap, r));

	// Texture
	auto regTexture = ClassRegister::New(engine, "Texture", 0, asOBJ_REF, "Drawable");
	regTexture->Method("TextureType getTextureType()", asFUNCTION(Texture_getTextureType), asCALL_CDECL_OBJFIRST);

	regTexture->Method("int getWidth(int mip = 0)", asFUNCTION(Texture_getWidth), asCALL_CDECL_OBJFIRST);
	regTexture->Method("int getHeight(int mip = 0)", asFUNCTION(Texture_getHeight), asCALL_CDECL_OBJFIRST);
	regTexture->Method("vec2 getDimensions(int mip = 0)", asFUNCTION(Texture_getDimensions), asCALL_CDECL_OBJFIRST);

	regTexture->Method("int getDepth(int mip = 0)", asFUNCTION(Texture_getDepth), asCALL_CDECL_OBJFIRST);

	regTexture->Method("int getLayerCount()", asFUNCTION(Texture_getLayerCount), asCALL_CDECL_OBJFIRST);
	regTexture->Method("int getMipmapCount()", asFUNCTION(Texture_getMipmapCount), asCALL_CDECL_OBJFIRST);

	regTexture->Method("int getPixelWidth(int mip = 0)", asFUNCTION(Texture_getPixelWidth), asCALL_CDECL_OBJFIRST);
	regTexture->Method("int getPixelHeight(int mip = 0)", asFUNCTION(Texture_getPixelHeight), asCALL_CDECL_OBJFIRST);
	regTexture->Method("vec2 getPixelDimensions(int mip = 0)", asFUNCTION(Texture_getPixelDimensions), asCALL_CDECL_OBJFIRST);

	regTexture->Method("float getDPIScale()", asFUNCTION(Texture_getDPIScale), asCALL_CDECL_OBJFIRST);

	regTexture->Method("void setFilter(const TextureFilter &in f)", asFUNCTION(Texture_setFilter), asCALL_CDECL_OBJFIRST);
	regTexture->Method("const TextureFilter &getFilter()", asFUNCTION(Texture_getFilter), asCALL_CDECL_OBJFIRST);

	regTexture->Method("void setMipmapFilter(FilterMode mode)", asFUNCTION(Texture_setMipmapFilter), asCALL_CDECL_OBJFIRST);
	regTexture->Method("FilterMode getMipmapFilter()", asFUNCTION(Texture_getMipmapFilter), asCALL_CDECL_OBJFIRST);

	regTexture->Method("bool setWrap(const TextureWrap &in w)", asFUNCTION(Texture_setWrap), asCALL_CDECL_OBJFIRST);
	regTexture->Method("const TextureWrap &getWrap()", asFUNCTION(Texture_getWrap), asCALL_CDECL_OBJFIRST);

	regTexture->Method("PixelFormat getFormat()", asFUNCTION(Texture_getFormat), asCALL_CDECL_OBJFIRST);
	regTexture->Method("bool isReadable()", asFUNCTION(Texture_isReadable), asCALL_CDECL_OBJFIRST);

	//regTexture->Method("void getDepthSampleMode()", asFUNCTION(Texture_getDepthSampleMode), asCALL_CDECL_OBJFIRST);
	//regTexture->Method("void setDepthSampleMode()", asFUNCTION(Texture_setDepthSampleMode), asCALL_CDECL_OBJFIRST);

	engine->SetDefaultNamespace("");
}
