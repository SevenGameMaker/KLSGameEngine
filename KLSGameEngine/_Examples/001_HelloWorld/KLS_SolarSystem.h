// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Level.h"
#include "KLS_Colors.h"
#include "KLS_Entity.h"

#include <filesystem>
namespace fs = std::filesystem;

// unclutter the global namespace
namespace KLS
{
	struct KLS_SolarSystemInfo
	{
		KLS_SolarSystemInfo() {}
		KLS_SolarSystemInfo(std::string name, KLS_Transform t) : Name(name), Transform(t) {}

		std::string Name;
		KLS_Transform Transform;

		float SunSize = 200.0f;

		glm::vec4 PlanetOrbitColor = KLS_LIGHTWHITE;
		int PlanetCountMin = 3;
		int PlanetCountMax = 8;
		float PlanetSizeMin = 0.3f;
		float PlanetSizeMax = 0.5f;
		float PlanetOrbitSpeedMin = 0.01f;
		float PlanetOrbitSpeedMax = 0.03f;

		float MoonOffsetFromPlanet = 0.1f;
		int MoonCountMin = 0;
		int MoonCountMax = 3;
		glm::vec4 MoonOrbitColor = KLS_LIGHTBLUE;
		float MoonSizeMin = 0.03f;
		float MoonSizeMax = 0.08f;
		float MoonOrbitSpeedMin = 0.01f;
		float MoonOrbitSpeedMax = 0.03f;

	};

	struct Planet
	{
		std::string name;
		float scale;
		float distance;
		float orbitSpeed;
		float startTime;
		std::vector<Planet> moons;
		Planet(std::string _name, float _scale, float distance, float speed, float time): name(std::move(_name)), scale(_scale), distance(distance), orbitSpeed(speed), startTime(time) {}
	};

	class KLS_SolarSystem
	{
	private:
		KLS_Entity Entity{ nullptr, entt::null };
		KLS_Level* Level = nullptr;
		KLS_SolarSystemInfo Info;
		std::vector<std::string> m_PlanetTextures;
		std::vector<std::string> m_MoonTextures;

	public:
		KLS_SolarSystem(KLS_Level* level, KLS_SolarSystemInfo info);

		KLS_Entity createRandomSolarSystem();
		KLS_Entity getEntity() { return Entity; }


	private:
		KLS_SolarSystem() = default;

		// planets and moon creation methods
		KLS_Entity createSun(KLS_Entity parent, std::string name, KLS_Transform transform, glm::vec4 color);
		KLS_Entity createFlightPath(KLS_Entity parent, std::string name, glm::vec3 scale, glm::vec4 color, float verticaloffset);
		KLS_Entity createPlanet(KLS_Entity parent, std::string name, KLS_Transform transform);
		KLS_Entity createMoon(KLS_Entity parent, std::string name, KLS_Transform transform);
	};




} // end namespace
