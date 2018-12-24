#include <angel_modules/image.h>

#include <modules/image/Image.h>

#define instance() (love::Module::getInstance<love::image::Image>(love::Module::M_IMAGE))

void RegisterImage(asIScriptEngine* engine)
{
	auto pImage = instance();
	if (pImage == nullptr) {
		pImage = new love::image::Image();
		love::Module::registerInstance(pImage);
	}

	engine->SetDefaultNamespace("angel::image");

	//TODO:
	// newImageData
	// newCompressedData
	// isCompressed
	// newCubeFaces

	engine->SetDefaultNamespace("");
}
