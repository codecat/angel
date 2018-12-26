#include <class_register.h>

std::vector<CClassRegister*> ClassRegister::g_instances;

static void* ScriptUpcast(void* p) { return p; }

CClassRegister::CClassRegister(asIScriptEngine* engine, const char* name, int byteSize, asDWORD flags, const char* parent)
{
	m_engine = engine;
	m_name = name;

	m_typeId = m_engine->RegisterObjectType(name, byteSize, flags);
	asITypeInfo* typeInfo = m_engine->GetTypeInfoById(m_typeId);
	typeInfo->SetUserData(this, AS_CLASSREGISTER_USERDATA);

	if (parent != nullptr) {
		asITypeInfo* parentTypeInfo = m_engine->GetTypeInfoByName(parent);
		if (parentTypeInfo == nullptr) {
			printf("WARNING! Couldn't find parent with name '%s'!\n", parent);
			return;
		}

		m_parent = (CClassRegister*)parentTypeInfo->GetUserData(AS_CLASSREGISTER_USERDATA);
		if (m_parent != nullptr) {
			if (m_parent != this) {
				m_parent->RegisterChild(this);
			} else {
				printf("WARNING! Can't inherit from itself, '%s'!\n", parent);
			}
		} else {
			printf("WARNING! Couldn't find CClassRegister userdata on class '%s'!\n", parent);
		}
	} else {
		m_parent = nullptr;
	}
}

CClassRegister::~CClassRegister()
{
}

void CClassRegister::Property(const char *declaration, int byteOffset, int compositeOffset, bool isCompositeIndirect)
{
	m_engine->RegisterObjectProperty(m_name.c_str(), declaration, byteOffset, compositeOffset, isCompositeIndirect);

	SProperty newProperty;
	newProperty.m_declaration = declaration;
	newProperty.m_byteOffset = byteOffset;
	newProperty.m_compositeOffset = compositeOffset;
	newProperty.m_isCompositeIndirect = isCompositeIndirect;
	m_properties.emplace_back(newProperty);
}

void CClassRegister::Method(const char *declaration, const asSFuncPtr &funcPointer, asDWORD callConv, void *auxiliary, int compositeOffset, bool isCompositeIndirect)
{
	m_engine->RegisterObjectMethod(m_name.c_str(), declaration, funcPointer, callConv, auxiliary, compositeOffset, isCompositeIndirect);

	SMethod newMethod;
	newMethod.m_declaration = declaration;
	newMethod.m_funcPointer = funcPointer;
	newMethod.m_callConv = callConv;
	newMethod.m_auxiliary = auxiliary;
	newMethod.m_compositeOffset = compositeOffset;
	newMethod.m_isCompositeIndirect = isCompositeIndirect;
	m_methods.emplace_back(newMethod);
}

void CClassRegister::Behavior(asEBehaviours behaviour, const char *declaration, const asSFuncPtr &funcPointer, asDWORD callConv, void *auxiliary, int compositeOffset, bool isCompositeIndirect)
{
	m_engine->RegisterObjectBehaviour(m_name.c_str(), behaviour, declaration, funcPointer, callConv, auxiliary, compositeOffset, isCompositeIndirect);

	SBehavior newBehavior;
	newBehavior.m_behavior = behaviour;
	newBehavior.m_declaration = declaration;
	newBehavior.m_funcPointer = funcPointer;
	newBehavior.m_callConv = callConv;
	newBehavior.m_auxiliary = auxiliary;
	newBehavior.m_compositeOffset = compositeOffset;
	newBehavior.m_isCompositeIndirect = isCompositeIndirect;
	m_behaviors.emplace_back(newBehavior);
}

void CClassRegister::RegisterChild(CClassRegister* child)
{
	if (m_parent != nullptr) {
		m_parent->RegisterChild(child);
	}

	for (auto &property : m_properties) {
		m_engine->RegisterObjectProperty(child->m_name.c_str(), property.m_declaration.c_str(), property.m_byteOffset, property.m_compositeOffset, property.m_isCompositeIndirect);
	}

	for (auto &method : m_methods) {
		m_engine->RegisterObjectMethod(child->m_name.c_str(), method.m_declaration.c_str(), method.m_funcPointer, method.m_callConv, method.m_auxiliary, method.m_compositeOffset, method.m_isCompositeIndirect);
	}

	for (auto &behavior : m_behaviors) {
		// Skip factories
		if (behavior.m_behavior == asBEHAVE_FACTORY) {
			continue;
		}
		m_engine->RegisterObjectBehaviour(child->m_name.c_str(), behavior.m_behavior, behavior.m_declaration.c_str(), behavior.m_funcPointer, behavior.m_callConv, behavior.m_auxiliary, behavior.m_compositeOffset, behavior.m_isCompositeIndirect);
	}

	std::string declUpcast = m_name + "@+ opImplCast()";
	m_engine->RegisterObjectMethod(child->m_name.c_str(), declUpcast.c_str(), asFUNCTION(ScriptUpcast), asCALL_CDECL_OBJFIRST);

	//TODO: Explicit downcast
}

CClassRegister* ClassRegister::New(asIScriptEngine* engine, const char* name, int byteSize, asDWORD flags, const char* parent)
{
	auto newInstance = new CClassRegister(engine, name, byteSize, flags, parent);
	g_instances.emplace_back(newInstance);
	return newInstance;
}

void ClassRegister::Clear()
{
	for (auto instance : g_instances) {
		delete instance;
	}
	g_instances.clear();
}
