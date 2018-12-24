#include <angel_modules/data.h>

#include <class_register.h>

#include <modules/data/DataModule.h>
#include <modules/data/CompressedData.h>

#define instance() (love::Module::getInstance<love::data::DataModule>(love::Module::M_DATA))

static love::Data* module_newDataView(love::Data* data, uint64_t offset, uint64_t size) { return instance()->newDataView(data, (size_t)offset, (size_t)size); }
static love::Data* module_newByteData(uint64_t size) { return instance()->newByteData((size_t)size); }

static love::data::CompressedData* module_compress(love::data::Compressor::Format format, love::Data* data, int level)
{
	const char* rawbytes = (const char*)data->getData();
	size_t rawsize = data->getSize();
	return love::data::compress(format, rawbytes, rawsize, level);
}
static love::Data* module_decompress(love::data::CompressedData* data)
{
	size_t rawsize;
	char* rawbytes = love::data::decompress(data, rawsize);
	return new love::data::ByteData(rawbytes, rawsize);
}

static love::Data* module_encode(love::data::EncodeFormat format, love::Data* src, uint64_t linelen)
{
	size_t rawsize;
	char* rawbytes = love::data::encode(format, (const char*)src->getData(), src->getSize(), rawsize, linelen);
	return new love::data::ByteData(rawbytes, rawsize);
}
static love::Data* module_decode(love::data::EncodeFormat format, love::Data* src)
{
	size_t rawsize;
	char* rawbytes = love::data::decode(format, (const char*)src->getData(), src->getSize(), rawsize);
	return new love::data::ByteData(rawbytes, rawsize);
}

static std::string module_hash(love::data::HashFunction::Function function, love::Data* input)
{
	return love::data::hash(function, input);
}

//static void module_pack() { instance()->pack(); }
//static void module_unpack() { instance()->unpack(); }
//static void module_getPackedSize() { instance()->getPackedSize(); }

void RegisterData(asIScriptEngine* engine)
{
	auto pData = instance();
	if (pData == nullptr) {
		pData = new love::data::DataModule();
		love::Module::registerInstance(pData);
	}

	engine->SetDefaultNamespace("angel::data");

	// CompressorFormat
	engine->RegisterEnum("CompressorFormat");
	engine->RegisterEnumValue("CompressorFormat", "LZ4", love::data::Compressor::FORMAT_LZ4);
	engine->RegisterEnumValue("CompressorFormat", "Zlib", love::data::Compressor::FORMAT_ZLIB);
	engine->RegisterEnumValue("CompressorFormat", "Gzip", love::data::Compressor::FORMAT_GZIP);
	engine->RegisterEnumValue("CompressorFormat", "Deflate", love::data::Compressor::FORMAT_DEFLATE);

	// EncodeFormat
	engine->RegisterEnum("EncodeFormat");
	engine->RegisterEnumValue("EncodeFormat", "Base64", love::data::ENCODE_BASE64);
	engine->RegisterEnumValue("EncodeFormat", "Hex", love::data::ENCODE_HEX);

	// HashFunction
	engine->RegisterEnum("HashFunction");
	engine->RegisterEnumValue("HashFunction", "MD5", love::data::HashFunction::FUNCTION_MD5);
	engine->RegisterEnumValue("HashFunction", "SHA1", love::data::HashFunction::FUNCTION_SHA1);
	engine->RegisterEnumValue("HashFunction", "SHA224", love::data::HashFunction::FUNCTION_SHA224);
	engine->RegisterEnumValue("HashFunction", "SHA256", love::data::HashFunction::FUNCTION_SHA256);
	engine->RegisterEnumValue("HashFunction", "SHA384", love::data::HashFunction::FUNCTION_SHA384);
	engine->RegisterEnumValue("HashFunction", "SHA512", love::data::HashFunction::FUNCTION_SHA512);

	// CompressedData
	auto regCompressedData = ClassRegister::New(engine, "CompressedData", 0, asOBJ_REF, "Data");

	// Module
	engine->RegisterGlobalFunction("Data@ newDataView(Data@ data, uint64 offset, uint64 size)", asFUNCTION(module_newDataView), asCALL_CDECL);
	engine->RegisterGlobalFunction("Data@ newByteData(uint64 size)", asFUNCTION(module_newByteData), asCALL_CDECL);

	engine->RegisterGlobalFunction("CompressedData@ compress(CompressorFormat format, Data@ data, int level = -1)", asFUNCTION(module_compress), asCALL_CDECL);
	engine->RegisterGlobalFunction("Data@ decompress(CompressedData@ data)", asFUNCTION(module_decompress), asCALL_CDECL);

	engine->RegisterGlobalFunction("Data@ encode(EncodeFormat format, Data@ src, uint64 linelen = 0)", asFUNCTION(module_encode), asCALL_CDECL);
	engine->RegisterGlobalFunction("Data@ decode(EncodeFormat format, Data@ src)", asFUNCTION(module_decode), asCALL_CDECL);

	engine->RegisterGlobalFunction("string hash(HashFunction function, Data@ input)", asFUNCTION(module_hash), asCALL_CDECL);

	//engine->RegisterGlobalFunction("void pack()", asFUNCTION(module_pack), asCALL_CDECL);
	//engine->RegisterGlobalFunction("void unpack()", asFUNCTION(module_unpack), asCALL_CDECL);
	//engine->RegisterGlobalFunction("void getPackedSize()", asFUNCTION(module_getPackedSize), asCALL_CDECL);

	engine->SetDefaultNamespace("");
}
