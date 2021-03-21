#pragma once

namespace sonic
{
	template<typename C1, typename C2>
	struct GroupView;

	struct UIComponent;

	struct UIRendererComponent;

	class UIRenderer
	{
		void init()
		{

		}

		void update(float deltaSeconds, GroupView<UIComponent, UIRendererComponent>& components)
		{

		}
	};

} // namespace sonic
