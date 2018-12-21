#include <angelscript.h>

#include <new>

template<typename T> static void ScriptCtor(T* self) { new (self) T; }
template<typename T> static void ScriptCopyCtor(T* self, const T &copy) { new (self) T(copy); }
template<typename T> static void ScriptDtor(T* self) { self->~T(); }
