#include <angel_common/object.h>

#include <common/Object.h>

#include <class_register.h>

//#define DEBUG_REFCOUNTS

#if defined(DEBUG_REFCOUNTS)
#include <modules/event/Event.h>

// Source: https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname
#include <memory>
#include <cxxabi.h>
std::string demangle(const char* name)
{
	int status = -4; // some arbitrary value to eliminate the compiler warning

	// enable c++11 by passing the flag -std=c++11 to g++
	std::unique_ptr<char, void(*)(void*)> res {
		abi::__cxa_demangle(name, NULL, NULL, &status),
		std::free
	};

	return (status==0) ? res.get() : name ;
}

static void Object_retain(love::Object* self)
{
	auto &ti = typeid(*self);
	if (ti == typeid(love::event::Message)) {
		self->retain();
		return;
	}
	std::string tiName = demangle(ti.name());

	int count = self->getReferenceCount();
	printf("  Retaining object %s at %p to refcount %d:\n", tiName.c_str(), self, count + 1);

	asIScriptContext* ctx = asGetActiveContext();
	if (ctx != nullptr) {
		for (asUINT i = 0; i < ctx->GetCallstackSize(); i++) {
			asIScriptFunction* frameFunc = ctx->GetFunction(i);
			int frameLine = ctx->GetLineNumber(i);
			printf("    %s (Line %d)\n", frameFunc->GetDeclaration(true, true), frameLine);
		}
	}

	self->retain();
	if (self->getReferenceCount() > 100) {
		printf("WARNING: Ref count for object %p is %d!\n", self, self->getReferenceCount());
	}
}

static void Object_release(love::Object* self)
{
	auto &ti = typeid(*self);
	if (ti == typeid(love::event::Message)) {
		self->retain();
		return;
	}
	std::string tiName = demangle(ti.name());

	if (self->getReferenceCount() == 1) {
		printf("| DELETING object %s at %p: !!!\n", tiName.c_str(), self);
	} else {
		printf("| Releasing object %s at %p to refcount %d:\n", tiName.c_str(), self, self->getReferenceCount() - 1);
	}

	asIScriptContext* ctx = asGetActiveContext();
	if (ctx != nullptr) {
		for (asUINT i = 0; i < ctx->GetCallstackSize(); i++) {
			asIScriptFunction* frameFunc = ctx->GetFunction(i);
			int frameLine = ctx->GetLineNumber(i);
			printf("|   %s (Line %d)\n", frameFunc->GetDeclaration(true, true), frameLine);
		}
	}

	self->release();
}
#endif

void RegisterObject(asIScriptEngine* engine)
{
	auto regObject = ClassRegister::New(engine, "Object", 0, asOBJ_REF);

#if defined(DEBUG_REFCOUNTS)
	regObject->Behavior(asBEHAVE_ADDREF, "void f()", asFUNCTION(Object_retain), asCALL_CDECL_OBJFIRST);
	regObject->Behavior(asBEHAVE_RELEASE, "void f()", asFUNCTION(Object_release), asCALL_CDECL_OBJFIRST);
#else
	regObject->Behavior(asBEHAVE_ADDREF, "void f()", asMETHOD(love::Object, retain), asCALL_THISCALL);
	regObject->Behavior(asBEHAVE_RELEASE, "void f()", asMETHOD(love::Object, release), asCALL_THISCALL);
#endif
}
