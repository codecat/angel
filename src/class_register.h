#pragma once

#include <angelscript.h>

#include <string>
#include <vector>

#define AS_CLASSREGISTER_USERDATA 300

class CClassRegister
{
private:
	asIScriptEngine* m_engine;
	std::string m_name;
	int m_typeId;

	CClassRegister* m_parent;

	struct SReg
	{
		std::string m_declaration;
		int m_compositeOffset;
		bool m_isCompositeIndirect;
	};

	struct SProperty : public SReg
	{
		int m_byteOffset;
	};

	struct SMethod : public SReg
	{
		asSFuncPtr m_funcPointer;
		asDWORD m_callConv;
		void* m_auxiliary;
	};

	struct SBehavior : public SMethod
	{
		asEBehaviours m_behavior;
	};

	std::vector<SProperty> m_properties;
	std::vector<SMethod> m_methods;
	std::vector<SBehavior> m_behaviors;

public:
	CClassRegister(asIScriptEngine* engine, const char* name, int byteSize, asDWORD flags, const char* parent = nullptr);
	~CClassRegister();

	void Property(const char *declaration, int byteOffset, int compositeOffset = 0, bool isCompositeIndirect = false);
	void Method(const char *declaration, const asSFuncPtr &funcPointer, asDWORD callConv, void *auxiliary = 0, int compositeOffset = 0, bool isCompositeIndirect = false);
	void Behavior(asEBehaviours behaviour, const char *declaration, const asSFuncPtr &funcPointer, asDWORD callConv, void *auxiliary = 0, int compositeOffset = 0, bool isCompositeIndirect = false);

private:
	void RegisterChild(CClassRegister* child);
};

namespace ClassRegister
{
	extern std::vector<CClassRegister*> g_instances;

	CClassRegister* New(asIScriptEngine* engine, const char* name, int byteSize, asDWORD flags, const char* parent = nullptr);
	void Clear();
}
