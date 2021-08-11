#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

namespace engine
{
	class LightComponent;

	class LightManager : public Singleton<LightManager>
	{
		static LightManager ms_instance;
	public:
		void addLight(LightComponent* light);
		void removeLight(LightComponent* light);

		std::vector<LightComponent*>& getLights() { return m_lights; }

	private:
		std::vector<LightComponent*> m_lights;
	};
}

#endif // !LIGHTCONTROLLER_H
