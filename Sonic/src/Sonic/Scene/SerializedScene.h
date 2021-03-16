#pragma once
#include "Sonic/Scene/Scene.h"

namespace Sonic {

	class SerializedScene : public Scene
	{
	public:
		SerializedScene(const String& relativeFilePath)
			: m_RelativeFilePath(relativeFilePath)
		{
		}

		void OnLoad() override;

		void OnSave() override;

	private:
		String m_RelativeFilePath;
	};

}
