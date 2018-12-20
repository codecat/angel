#include <angel_modules/filesystem.h>

#include <modules/filesystem/Filesystem.h>
#include <modules/filesystem/physfs/Filesystem.h>

#include <scriptarray/scriptarray.h>

#define instance() (love::Module::getInstance<love::filesystem::Filesystem>(love::Module::M_FILESYSTEM))

static void setFused(bool fused) { instance()->setFused(fused); }
static bool isFused() { return instance()->isFused(); }
static void _setAndroidSaveExternal(bool useExternal) { instance()->setAndroidSaveExternal(useExternal); }
static bool setIdentity(const std::string &ident, bool appendToPath) { return instance()->setIdentity(ident.c_str(), appendToPath); }
static std::string getIdentity() { return instance()->getIdentity(); }
static bool setSource(const std::string &source) { return instance()->setSource(source.c_str()); }
static std::string getSource() { return instance()->getSource(); }
static bool mount(const std::string &archive, const std::string &mountpoint, bool appendToPath) { return instance()->mount(archive.c_str(), mountpoint.c_str(), appendToPath); }
//static bool unmount() { }
//static void newFile() { }
static std::string getWorkingDirectory() { return instance()->getWorkingDirectory(); }
static std::string getUserDirectory() { return instance()->getUserDirectory(); }
static std::string getAppdataDirectory() { return instance()->getAppdataDirectory(); }
static std::string getSaveDirectory() { return instance()->getSaveDirectory(); }
static std::string getSourceBaseDirectory() { return instance()->getSourceBaseDirectory(); }
static std::string getRealDirectory(const std::string &filename) { return instance()->getRealDirectory(filename.c_str()); }
static std::string getExecutablePath() { return instance()->getExecutablePath(); }
static bool createDirectory(const std::string &dir) { return instance()->createDirectory(dir.c_str()); }
static bool remove(const std::string &file) { return instance()->remove(file.c_str()); }
//static void read() { }
//static void write() { }
//static void append() { }
static void getDirectoryItems(const std::string &dir, CScriptArray &items)
{
	std::vector<std::string> strings;
	instance()->getDirectoryItems(dir.c_str(), strings);

	size_t startIndex = items.GetSize();
	items.Reserve(strings.size());
	for (size_t i = 0; i < strings.size(); i++) {
		*(std::string*)items.At(startIndex + i) = strings[i];
	}
}
//static void lines() { }
//static void load() { }
static bool getInfo(const std::string &filepath, love::filesystem::Filesystem::Info &info) { return instance()->getInfo(filepath.c_str(), info); }
static void setSymlinksEnabled(bool enable) { instance()->setSymlinksEnabled(enable); }
static bool areSymlinksEnabled() { return instance()->areSymlinksEnabled(); }
//static void newFileData() { }
//static void getRequirePath() { }
//static void setRequirePath() { }
//static void getCRequirePath() { }
//static void setCRequirePath() { }

void RegisterFilesystem(asIScriptEngine* engine, const char* argv0)
{
	auto pfs = instance();
	if (pfs == nullptr) {
		pfs = new love::filesystem::physfs::Filesystem();
		love::Module::registerInstance(pfs);
	}

	pfs->init(argv0);

	engine->SetDefaultNamespace("filesystem");

	// FileType
	engine->RegisterEnum("FileType");
	engine->RegisterEnumValue("FileType", "File", love::filesystem::Filesystem::FILETYPE_FILE);
	engine->RegisterEnumValue("FileType", "Directory", love::filesystem::Filesystem::FILETYPE_DIRECTORY);
	engine->RegisterEnumValue("FileType", "Symlink", love::filesystem::Filesystem::FILETYPE_SYMLINK);
	engine->RegisterEnumValue("FileType", "Other", love::filesystem::Filesystem::FILETYPE_OTHER);

	// Info
	engine->RegisterObjectType("Info", sizeof(love::filesystem::Filesystem::Info), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<love::filesystem::Filesystem::Info>());
	engine->RegisterObjectProperty("Info", "int64 size", asOFFSET(love::filesystem::Filesystem::Info, size));
	engine->RegisterObjectProperty("Info", "int64 modtime", asOFFSET(love::filesystem::Filesystem::Info, modtime));
	engine->RegisterObjectProperty("Info", "FileType type", asOFFSET(love::filesystem::Filesystem::Info, type));

	// FileData
	engine->RegisterObjectType("FileData", 0, asOBJ_REF);
	engine->RegisterObjectBehaviour("FileData", asBEHAVE_ADDREF, "void f()", asMETHOD(love::Object, retain), asCALL_THISCALL);
	engine->RegisterObjectBehaviour("FileData", asBEHAVE_RELEASE, "void f()", asMETHOD(love::Object, release), asCALL_THISCALL);

	engine->RegisterObjectMethod("FileData", "FileData@ clone() const", asMETHOD(love::filesystem::FileData, clone), asCALL_THISCALL);
	//TODO: data (void*) & size (64 or 32 bit size_t)

	engine->RegisterObjectMethod("FileData", "const string &getFilename() const", asMETHOD(love::filesystem::FileData, getFilename), asCALL_THISCALL);
	engine->RegisterObjectMethod("FileData", "const string &getExtension() const", asMETHOD(love::filesystem::FileData, getExtension), asCALL_THISCALL);
	engine->RegisterObjectMethod("FileData", "const string &getName() const", asMETHOD(love::filesystem::FileData, getName), asCALL_THISCALL);

	// Global functions
	engine->RegisterGlobalFunction("string getExecutablePath()", asFUNCTION(getExecutablePath), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool getInfo(const string &in filepath, Info &out info)", asFUNCTION(getInfo), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setSymlinksEnabled(bool enable)", asFUNCTION(setSymlinksEnabled), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool areSymlinksEnabled()", asFUNCTION(areSymlinksEnabled), asCALL_CDECL);

	engine->SetDefaultNamespace("");
}
