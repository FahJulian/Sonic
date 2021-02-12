#pragma once
#include "Script.h"

namespace Sonic {

	struct ScriptComponent
	{
		Script* script;

		ScriptComponent(Script* script)
			: script(script)
		{
		}
	};

}
