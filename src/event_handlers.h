#pragma once

#include <modules/event/Event.h>

#include <angelscript.h>

namespace EventHandlers
{
	void FindGameEvents(asIScriptModule* mod);
	void ScriptGameEvent(love::event::Message* msg);
}
