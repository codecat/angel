#include <angel_modules/filesystem.h>

#include <modules/filesystem/Filesystem.h>
#include <modules/filesystem/physfs/Filesystem.h>

#include <scriptarray/scriptarray.h>

#define instance() (love::Module::getInstance<love::filesystem::Filesystem>(love::Module::M_FILESYSTEM))

// File
static int64_t file_getSize(love::filesystem::File* self) { return self->getSize(); }
static bool file_open(love::filesystem::File* self, love::filesystem::File::Mode mode) { return self->open(mode); }
static bool file_close(love::filesystem::File* self) { return self->close(); }
static bool file_isOpen(love::filesystem::File* self) { return self->isOpen(); }
static love::filesystem::FileData* file_read(love::filesystem::File* self, int64_t size) { return self->read(size); }
static bool file_write(love::filesystem::File* self, const love::filesystem::FileData* data, int64_t size) { return self->write(data, size); }
static bool file_flush(love::filesystem::File* self) { return self->flush(); }
static bool file_isEOF(love::filesystem::File* self) { return self->isEOF(); }
static int64_t file_tell(love::filesystem::File* self) { return self->tell(); }
static bool file_seek(love::filesystem::File* self, uint64_t pos) { return self->seek(pos); }
//static void file_lines(love::filesystem::File* self) { }
static bool file_setBuffer(love::filesystem::File* self, love::filesystem::File::BufferMode bufmode, int64_t size) { return self->setBuffer(bufmode, size); }
static love::filesystem::File::BufferMode file_getBuffer(love::filesystem::File* self, int64_t &size) { return self->getBuffer(size); }
static love::filesystem::File::Mode file_getMode(love::filesystem::File* self) { return self->getMode(); }
static const std::string &file_getFilename(love::filesystem::File* self) { return self->getFilename(); }
static std::string file_getExtension(love::filesystem::File* self) { return self->getExtension(); }

// Module
static void module_setFused(bool fused) { instance()->setFused(fused); }
static bool module_isFused() { return instance()->isFused(); }
static void module__setAndroidSaveExternal(bool useExternal) { instance()->setAndroidSaveExternal(useExternal); }
static bool module_setIdentity(const std::string &ident, bool appendToPath) { return instance()->setIdentity(ident.c_str(), appendToPath); }
static std::string module_getIdentity() { return instance()->getIdentity(); }
static bool module_setSource(const std::string &source) { return instance()->setSource(source.c_str()); }
static std::string module_getSource() { return instance()->getSource(); }
static bool module_mount(const std::string &archive, const std::string &mountpoint, bool appendToPath) { return instance()->mount(archive.c_str(), mountpoint.c_str(), appendToPath); }
//static bool module_unmount() { }
static love::filesystem::File* module_newFile(const std::string &filename) { return instance()->newFile(filename.c_str()); }
static std::string module_getWorkingDirectory() { return instance()->getWorkingDirectory(); }
static std::string module_getUserDirectory() { return instance()->getUserDirectory(); }
static std::string module_getAppdataDirectory() { return instance()->getAppdataDirectory(); }
static std::string module_getSaveDirectory() { return instance()->getSaveDirectory(); }
static std::string module_getSourceBaseDirectory() { return instance()->getSourceBaseDirectory(); }
static std::string module_getRealDirectory(const std::string &filename) { return instance()->getRealDirectory(filename.c_str()); }
static std::string module_getExecutablePath() { return instance()->getExecutablePath(); }
static bool module_createDirectory(const std::string &dir) { return instance()->createDirectory(dir.c_str()); }
static bool module_remove(const std::string &file) { return instance()->remove(file.c_str()); }
//static void module_read() { }
//static void module_write() { }
//static void module_append() { }
static void module_getDirectoryItems(const std::string &dir, CScriptArray &items)
{
	std::vector<std::string> strings;
	instance()->getDirectoryItems(dir.c_str(), strings);

	size_t startIndex = items.GetSize();
	items.Reserve(strings.size());
	for (size_t i = 0; i < strings.size(); i++) {
		*(std::string*)items.At(startIndex + i) = strings[i];
	}
}
//static void module_lines() { }
//static void module_load() { }
static bool module_getInfo(const std::string &filepath, love::filesystem::Filesystem::Info &info) { return instance()->getInfo(filepath.c_str(), info); }
static void module_setSymlinksEnabled(bool enable) { instance()->setSymlinksEnabled(enable); }
static bool module_areSymlinksEnabled() { return instance()->areSymlinksEnabled(); }
//static love::filesystem::FileData* module_newFileData() { }
//static void module_getRequirePath() { }
//static void module_setRequirePath() { }
//static void module_getCRequirePath() { }
//static void module_setCRequirePath() { }

void RegisterFilesystem(asIScriptEngine* engine, const char* argv0)
{
	auto pFilesystem = instance();
	if (pFilesystem == nullptr) {
		pFilesystem = new love::filesystem::physfs::Filesystem();
		love::Module::registerInstance(pFilesystem);
	}

	pFilesystem->init(argv0);

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

	// Mode
	engine->RegisterEnum("Mode");
	engine->RegisterEnumValue("Mode", "Closed", love::filesystem::File::MODE_CLOSED);
	engine->RegisterEnumValue("Mode", "Read", love::filesystem::File::MODE_READ);
	engine->RegisterEnumValue("Mode", "Write", love::filesystem::File::MODE_WRITE);
	engine->RegisterEnumValue("Mode", "Append", love::filesystem::File::MODE_APPEND);

	// BufferMode
	engine->RegisterEnum("BufferMode");
	engine->RegisterEnumValue("BufferMode", "None", love::filesystem::File::BUFFER_NONE);
	engine->RegisterEnumValue("BufferMode", "Line", love::filesystem::File::BUFFER_LINE);
	engine->RegisterEnumValue("BufferMode", "Full", love::filesystem::File::BUFFER_FULL);

	// File
	engine->RegisterObjectType("File", 0, asOBJ_REF);
	engine->RegisterObjectBehaviour("File", asBEHAVE_ADDREF, "void f()", asMETHOD(love::Object, retain), asCALL_THISCALL);
	engine->RegisterObjectBehaviour("File", asBEHAVE_RELEASE, "void f()", asMETHOD(love::Object, release), asCALL_THISCALL);

	engine->RegisterObjectMethod("File", "int64 getSize()", asFUNCTION(file_getSize), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("File", "bool open(Mode mode)", asFUNCTION(file_open), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("File", "bool close()", asFUNCTION(file_close), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("File", "bool isOpen()", asFUNCTION(file_isOpen), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("File", "FileData@ read(int64 size)", asFUNCTION(file_read), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("File", "bool write(FileData@ data, int64 size)", asFUNCTION(file_write), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("File", "bool flush()", asFUNCTION(file_flush), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("File", "bool isEOF()", asFUNCTION(file_isEOF), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("File", "int64 tell()", asFUNCTION(file_tell), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("File", "bool seek(uint64 pos)", asFUNCTION(file_seek), asCALL_CDECL_OBJFIRST);
	//engine->RegisterObjectMethod("File", "", asFUNCTION(file_), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("File", "bool setBuffer(BufferMode bufmode, int64 size)", asFUNCTION(file_setBuffer), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("File", "BufferMode getBuffer(int64 &out size)", asFUNCTION(file_getBuffer), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("File", "Mode getMode()", asFUNCTION(file_getMode), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("File", "const string &getFilename()", asFUNCTION(file_getFilename), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod("File", "string getExtension()", asFUNCTION(file_getExtension), asCALL_CDECL_OBJFIRST);

	// Module
	engine->RegisterGlobalFunction("void setFused(bool fused)", asFUNCTION(module_setFused), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool isFused()", asFUNCTION(module_isFused), asCALL_CDECL);
	engine->RegisterGlobalFunction("void _setAndroidSaveExternal(bool useExternal)", asFUNCTION(module__setAndroidSaveExternal), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool setIdentity(const string &in ident, bool appendToPath = false)", asFUNCTION(module_setIdentity), asCALL_CDECL);
	engine->RegisterGlobalFunction("string getIdentity()", asFUNCTION(module_getIdentity), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool setSource(const string &in source)", asFUNCTION(module_setSource), asCALL_CDECL);
	engine->RegisterGlobalFunction("string getSource()", asFUNCTION(module_getSource), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool mount(const string &in archive, const string &in mountpoint, bool appendToPath = false)", asFUNCTION(module_mount), asCALL_CDECL);
	engine->RegisterGlobalFunction("File@ newFile(const string &in filename)", asFUNCTION(module_newFile), asCALL_CDECL);
	engine->RegisterGlobalFunction("string getWorkingDirectory()", asFUNCTION(module_getWorkingDirectory), asCALL_CDECL);
	engine->RegisterGlobalFunction("string getSaveDirectory()", asFUNCTION(module_getSaveDirectory), asCALL_CDECL);
	engine->RegisterGlobalFunction("string getSourceBaseDirectory()", asFUNCTION(module_getSourceBaseDirectory), asCALL_CDECL);
	engine->RegisterGlobalFunction("string getRealDirectory(const string &in filename)", asFUNCTION(module_getRealDirectory), asCALL_CDECL);
	engine->RegisterGlobalFunction("string getExecutablePath()", asFUNCTION(module_getExecutablePath), asCALL_CDECL);
	engine->RegisterGlobalFunction("void createDirectory(const string &in file)", asFUNCTION(module_createDirectory), asCALL_CDECL);
	engine->RegisterGlobalFunction("void remove(const string &in file)", asFUNCTION(module_remove), asCALL_CDECL);
	engine->RegisterGlobalFunction("void getDirectoryItems(const string &in dir, array<string> &out items)", asFUNCTION(module_getDirectoryItems), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool getInfo(const string &in filepath, Info &out info)", asFUNCTION(module_getInfo), asCALL_CDECL);
	engine->RegisterGlobalFunction("void setSymlinksEnabled(bool enable)", asFUNCTION(module_setSymlinksEnabled), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool areSymlinksEnabled()", asFUNCTION(module_areSymlinksEnabled), asCALL_CDECL);

	engine->SetDefaultNamespace("");
}
