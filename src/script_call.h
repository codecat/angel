#pragma once

#include <angelscript.h>

class CScriptCall
{
public:
	asIScriptContext* m_ctx;
	asIScriptFunction* m_func;

public:
	CScriptCall(asIScriptContext* ctx, asIScriptFunction* func);

	void SetArg(int index, int num);
	void SetArg(int index, double num);
	void SetArg(int index, void* obj);

	bool Execute();
};
