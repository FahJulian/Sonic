#pragma once
#include "Sonic/Scene/Components/UIComponents.h"


namespace Sonic {
	
	class Scene;

	namespace UI {

		UIComponent* getParent(Scene* scene, UIComponent* c);

		void setX(Scene* scene, UIComponent* c, float value, UIPositionConstraintsComponent* constraints);
		void setY(Scene* scene, UIComponent* c, float value, UIPositionConstraintsComponent* constraints);
		void setWidth(Scene* scene, UIComponent* c, float value, UISizeConstraintsComponent* constraints);
		void setHeight(Scene* scene, UIComponent* c, float value, UISizeConstraintsComponent* constraints);

		void setX(Scene* scene, Entity e, UIComponent* c, float value);
		void setY(Scene* scene, Entity e, UIComponent* c, float value);
		void setWidth(Scene* scene, Entity e, UIComponent* c, float value);
		void setHeight(Scene* scene, Entity e, UIComponent* c, float value);

		void setXRelative(Scene* scene, UIComponent* c, float value, UIPositionConstraintsComponent* constraints);
		void setYRelative(Scene* scene, UIComponent* c, float value, UIPositionConstraintsComponent* constraints);
		void setWidthRelative(Scene* scene, UIComponent* c, float value, UISizeConstraintsComponent* constraints);
		void setHeightRelative(Scene* scene, UIComponent* c, float value, UISizeConstraintsComponent* constraints);

		void setXRelative(Scene* scene, Entity e, UIComponent* c, float value);
		void setYRelative(Scene* scene, Entity e, UIComponent* c, float value);
		void setWidthRelative(Scene* scene, Entity e, UIComponent* c, float value);
		void setHeightRelative(Scene* scene, Entity e, UIComponent* c, float value);

		void recalculate(Scene* scene, UIComponent* c);

		void checkPosition(Scene* scene, UIComponent* c, UIPositionConstraintsComponent* constraints);
		void checkSize(Scene* scene, UIComponent* c, UISizeConstraintsComponent* constraints);

	}
}
