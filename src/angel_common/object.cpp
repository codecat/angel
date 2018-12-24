#include <angel_common/object.h>

#include <common/Object.h>

#include <class_register.h>

static void Object_retain(love::Object* self)
{
	/*
	int count = self->getReferenceCount();
	asIScriptContext* ctx = asGetActiveContext();
	printf("  Retaining object at %p with refcount %d:\n", self, count);
	for (asUINT i = 0; i < ctx->GetCallstackSize(); i++) {
		asIScriptFunction* frameFunc = ctx->GetFunction(i);
		int frameLine = ctx->GetLineNumber(i);
		printf("    %s (Line %d)\n", frameFunc->GetDeclaration(true, true), frameLine);
	}
	*/
	self->retain();
	if (self->getReferenceCount() > 100) {
		printf("WARNING: Ref count for object %p is %d!\n", self, self->getReferenceCount());
	}
}

static void Object_release(love::Object* self)
{
	/*
	int count = self->getReferenceCount();
	if (count - 1 <= 0) {
		asIScriptContext* ctx = asGetActiveContext();
		printf("| Releasing object at %p with refcount %d:\n", self, count);
		for (asUINT i = 0; i < ctx->GetCallstackSize(); i++) {
			asIScriptFunction* frameFunc = ctx->GetFunction(i);
			int frameLine = ctx->GetLineNumber(i);
			printf("|   %s (Line %d)\n", frameFunc->GetDeclaration(true, true), frameLine);
		}
	}
	*/
	self->release();
}

void RegisterObject(asIScriptEngine* engine)
{
	auto regObject = ClassRegister::New(engine, "Object", 0, asOBJ_REF);
	regObject->Behavior(asBEHAVE_ADDREF, "void f()", asFUNCTION(Object_retain), asCALL_CDECL_OBJFIRST);
	regObject->Behavior(asBEHAVE_RELEASE, "void f()", asFUNCTION(Object_release), asCALL_CDECL_OBJFIRST);
}
