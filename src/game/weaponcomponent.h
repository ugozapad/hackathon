#ifndef WEAPONCOMPONENT_H
#define WEAPONCOMPONENT_H


#include "engine/components/logiccomponent.h"

namespace engine {
	class WeaponComponent : public LogicComponent
	{
		ImplementObject(WeaponComponent, LogicComponent);
	public:
		WeaponComponent();
		~WeaponComponent();

		static void registerObject();

		virtual void onNodeSet(Node* node);
		virtual void update(float dt);
	};
}
#endif // ! WEAPONCOMPONENT_H