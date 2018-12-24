#include <script_call.h>

#include <cstdio>

CScriptCall::CScriptCall(asIScriptContext* ctx, asIScriptFunction* func)
{
	m_ctx = ctx;
	m_func = func;

	if (m_func != nullptr) {
		if (m_ctx->GetState() == asEXECUTION_ACTIVE) {
			m_ctx->PushState();
		}
		m_ctx->Prepare(m_func);
	}
}

void CScriptCall::SetArg(int index, int num) { if (m_func == nullptr) { return; } m_ctx->SetArgDWord(index, num); }
void CScriptCall::SetArg(int index, double num) { if (m_func == nullptr) { return; } m_ctx->SetArgDouble(index, num); }
void CScriptCall::SetArg(int index, void* obj) { if (m_func == nullptr) { return; } m_ctx->SetArgObject(index, obj); }

bool CScriptCall::Execute()
{
	if (m_func == nullptr) {
		return false;
	}

	m_ctx->Execute();
	if (m_ctx->GetState() == asEXECUTION_EXCEPTION) {
		printf("| Script exception: %s\n", m_ctx->GetExceptionString());
		for (asUINT i = 0; i < m_ctx->GetCallstackSize(); i++) {
			asIScriptFunction* frameFunc = m_ctx->GetFunction(i);
			int frameLine = m_ctx->GetLineNumber(i);
			printf("|   %s (Line %d)\n", frameFunc->GetDeclaration(true, true), frameLine);
		}
	}
	if (m_ctx->IsNested()) {
		m_ctx->PopState();
	} else {
		m_ctx->Unprepare();
	}
	return true;
}
