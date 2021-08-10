#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

namespace engine
{
	class LightComponent;

	class LightController : public Singleton<LightController>
	{
		static LightController ms_instance;
	public:
		void addLight(LightComponent* light);
		void removeLight(LightComponent* light);

	private:
		std::vector<LightComponent*> m_lights;
	};
}

#endif // !LIGHTCONTROLLER_H
