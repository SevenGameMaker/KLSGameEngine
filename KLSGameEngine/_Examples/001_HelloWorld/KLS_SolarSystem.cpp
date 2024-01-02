#include "KLS_SolarSystem.h"
#include "KLS_Math.h"
#include "_KLS_Driver.h"
#include "KLS_ResourceManager.h"
#include "KLS_Mesh.h"
#include "_KLS_ECS_Components.h"
#include "KLS_File.h"

namespace KLS
{
	KLS_SolarSystem::KLS_SolarSystem(KLS_Level* level, KLS_SolarSystemInfo info)
	{
		Level = level;
		Info = info;
		Entity = createRandomSolarSystem();
	}

	KLS_Entity KLS_SolarSystem::createRandomSolarSystem()
	{
		// create a generic entity that holds the entire solar system
		KLS_Entity e = Level->getECS()->createEntity(Level->getUniqueId(), Info.Name, Info.Transform);

		// define a few variabels to use
		glm::vec3 sunscale{ Info.SunSize };

		// create the sun
		KLS_Entity sun = createSun(e, "sun", KLS_Transform(glm::vec3(0), KLS_IDENTITY_QUAT, sunscale), glm::vec4(0.2, 0.04, 0.01, 1));

		// add the planets, then roll through and add the moons. just use default values for now
		std::vector<Planet> planets;
		int numberofplanets = getRandomInt(Info.PlanetCountMin, Info.PlanetCountMax);
		for (int x = 0; x < numberofplanets; x++)
		{
			// get a random size and create the planet
			float planetscale = getRandomFloat(Info.PlanetSizeMin, Info.PlanetSizeMax);
			planets.push_back(Planet("Planet", planetscale, 0.0f, getRandomFloat(Info.PlanetOrbitSpeedMin, Info.PlanetOrbitSpeedMax), getRandomFloat(0.f, 6.28f)));

			// get a random size and create the moons
			int numberofmoons = getRandomInt(Info.MoonCountMin, Info.MoonCountMax);
			for (int m = 0; m < numberofmoons; m++)
			{
				planets[x].moons.push_back(Planet("Moon", getRandomFloat(Info.MoonSizeMin,Info.MoonSizeMax),0.0f,getRandomFloat(Info.MoonOrbitSpeedMin,Info.MoonOrbitSpeedMax),getRandomFloat(0.0f,6.28f)));
			}
		}

		// start at the endge of the sun
		float planetdistance = sunscale.x / 2;

		// for each planet
		for (int x = 0; x < planets.size(); x++)
		{
			// start at the edge of the planet
			float moondistance = Info.MoonOffsetFromPlanet + planets[x].scale / 2 * sunscale.x;

			// for each moon
			for (int m = 0; m < planets[x].moons.size(); m++)
			{
				// add half the moon size
				moondistance += planets[x].moons[m].scale / 2 * planets[x].scale * sunscale.x + Info.MoonOffsetFromPlanet;
				// set the moon distance from the planet
				planets[x].moons[m].distance = moondistance;
				// add the other half of the moon size
				moondistance += planets[x].moons[m].scale / 2 * planets[x].scale * sunscale.x + Info.MoonOffsetFromPlanet;

				// add the moon size to the planet distance from the sun
				planetdistance += planets[x].moons[m].scale * planets[x].scale * sunscale.x + Info.MoonOffsetFromPlanet*2;
			}

			// add half the moon distance
			planetdistance += moondistance / 2;

			// add half of the planet
			planetdistance += planets[x].scale / 2 * sunscale.x;

			// set the planet distance
			planets[x].distance = planetdistance;

			// add the other half of the planet
			planetdistance += planets[x].scale / 2 * sunscale.x;

			// add half the moon distance
			planetdistance += moondistance / 2;

		}

		for (auto i = 0; i < planets.size(); i++)
		{
			// Create the planet entity at the default position (glm::vec3(0))
			KLS_Entity planet = createPlanet(sun, "planet", KLS_Transform(glm::vec3(0.0f), KLS_IDENTITY_QUAT, glm::vec3(planets[i].scale)));

			// Add the circular orbit component for the planet
			KLS_COMPONENT_FLYCIRCLE fc;
			fc.m_Center = glm::vec3(0);
			fc.m_Radius = planets[i].distance;
			fc.m_Speed = planets[i].orbitSpeed;
			fc.m_StartTime = planets[i].startTime;
			planet.addComponent<KLS_COMPONENT_FLYCIRCLE>(fc);

			createFlightPath(sun, "flight", glm::vec3(2 * planets[i].distance / sunscale.x), Info.PlanetOrbitColor, 0.0f);

			if (!planets[i].moons.empty())
			{
				for (auto m = 0; m < planets[i].moons.size(); m++)
				{
					// Create the planet entity at the default position (glm::vec3(0))
					KLS_Entity moon = createMoon(planet, "moon", KLS_Transform(glm::vec3(0.0f), KLS_IDENTITY_QUAT, glm::vec3(planets[i].moons[m].scale)));

					// Add the circular orbit component for the planet
					KLS_COMPONENT_FLYCIRCLE fc;
					fc.m_Center = glm::vec3(0.0f);
					fc.m_Radius = planets[i].moons[m].distance;
					fc.m_Speed = planets[i].moons[m].orbitSpeed;
					fc.m_StartTime = planets[i].moons[m].startTime;
					moon.addComponent<KLS_COMPONENT_FLYCIRCLE>(fc);

					float flightscale = 2 * planets[i].moons[m].distance / planets[i].scale / sunscale.x;
					createFlightPath(planet, "moonflight", glm::vec3(flightscale, 1.0f, flightscale), Info.MoonOrbitColor, 0.0f);
				}
			}
		}

		return e;
	}

	KLS_Entity KLS_SolarSystem::createSun(KLS_Entity parent, std::string name, KLS_Transform transform, glm::vec4 color)
	{
		KLS_Mesh* Mesh = Level->getDriver()->getResourceManager()->getMesh("../../../../_media/_assets/_models/_default/sphere.obj", KLSVT3D);
			Mesh->getMaterial(0).Texture_Diffuse[0] = "../../../../_media/_assets/_textures/_demo/_sun/noise.jpg";
			Mesh->getMaterial(0).ColorDiffuse = color;
			Mesh->getMaterial(0).MaterialType = KLSMT_SUN;
		KLS_Entity e = Level->getECS()->createEntity(Level->getUniqueId(), name, transform);
			e.addComponent<KLS_COMPONENT_MESH>(Mesh);

			KLS_Entity le = Level->getECS()->createEntity(Level->getUniqueId(), "Light", KLS_Transform(glm::vec3(0),KLS_IDENTITY_QUAT,glm::vec3(0.1f)));
				KLS_COMPONENT_LIGHT ld;
					le.addComponent<KLS_COMPONENT_LIGHT>(ld);
					KLS_Mesh* lightMesh = Level->getDriver()->getResourceManager()->getMesh("../../../../_media/_assets/_models/_default/light.obj", KLSVT3D);
						lightMesh->getMaterial(0).ColorDiffuse = ld.m_LightData.color;
						lightMesh->getMaterial(0).MaterialType = KLSMT_COLOR;
					le.addComponent<KLS_COMPONENT_MESH>(lightMesh);
			e.addChild(le);

		if (parent.getEntity() != entt::null) parent.addChild(e);
		return e;
	}

	KLS_Entity KLS_SolarSystem::createFlightPath(KLS_Entity parent, std::string name, glm::vec3 scale, glm::vec4 color, float verticaloffset)
	{
		KLS_Mesh* Mesh = Level->getDriver()->getResourceManager()->getMesh("../../../../_media/_assets/_models/_default/circle.obj", KLSVT3D);
		Mesh->getMaterial(0).Texture_Diffuse[0] = "../../../../_media/_assets/_textures/_demo/_sun/noise.jpg";
		Mesh->getMaterial(0).Primitive = KLS_PT_LINES;
		Mesh->getMaterial(0).ColorDiffuse = color;
		Mesh->getMaterial(0).MaterialType = KLSMT_COLOR;
		KLS_Entity e = Level->getECS()->createEntity(Level->getUniqueId(), name, KLS_Transform(glm::vec3(0.0f, verticaloffset, 0.0f), KLS_IDENTITY_QUAT, scale));
		e.addComponent<KLS_COMPONENT_MESH>(Mesh);
		if (parent.getEntity() != entt::null) parent.addChild(e);
		return e;
	}

	KLS_Entity KLS_SolarSystem::createPlanet(KLS_Entity parent, std::string name, KLS_Transform transform)
	{
		// build the list of textures if needed
		if (m_PlanetTextures.empty())
		{
			// make a list of planet textures
			std::string directoryPath = "../../../../_media/_assets/_textures/_demo/_planets/";
			std::string extension = ".png";
			for (const auto& entry : fs::directory_iterator(directoryPath))
			{
				if (entry.path().extension() == extension)
				{
					m_PlanetTextures.push_back(entry.path().string());
				}
			}
		}

		KLS_Mesh* Mesh = Level->getDriver()->getResourceManager()->getMesh("../../../../_media/_assets/_models/_default/sphere.obj", KLSVT3D);
		
		// select a random texture and calculate the normal map filename
		std::string filename = m_PlanetTextures[getRandomInt(0, static_cast<int>(m_PlanetTextures.size()) - 1)];
		std::string bumpfilename = addTextToFilename(filename, "normal");
			bumpfilename = addFolderToFilename(bumpfilename,"bumpmaps/");
			
			Mesh->getMaterial(0).Texture_Diffuse[0] = filename;
			Mesh->getMaterial(0).Texture_Diffuse[1] = bumpfilename;
			Mesh->getMaterial(0).MaterialType = KLSMT_BUMPMAP;
		KLS_Entity e = Level->getECS()->createEntity(Level->getUniqueId(), name, transform);
			e.addComponent<KLS_COMPONENT_MESH>(Mesh);
			e.addComponent<KLS_COMPONENT_ROTATE>(glm::vec3(0.0f, .1f, 0.0f));
		if (parent.getEntity() != entt::null) parent.addChild(e);


#if(1)
		// add a halo effect around the planet
			KLS_Mesh* haloMesh = Level->getDriver()->getResourceManager()->getMesh("../../../../_media/_assets/_models/_default/sphere.obj", KLSVT3D);
				haloMesh->getMaterial(0).ColorDiffuse = glm::vec4(.2,.2,.5,.4);
				haloMesh->getMaterial(0).MaterialType = KLSMT_COLOR;
				haloMesh->getMaterial(0).Transparency = 1;
				KLS_Transform ht;// = transform;
				ht.m_Scale += .03f;
				ht.m_Dirty = true;
			KLS_Entity eh = Level->getECS()->createEntity(Level->getUniqueId(), name, ht);
				eh.addComponent<KLS_COMPONENT_MESH>(haloMesh);
				e.addChild(eh);
#endif
		return e;
	}

	KLS_Entity KLS_SolarSystem::createMoon(KLS_Entity parent, std::string name, KLS_Transform transform)
	{
		// build the list of textures if needed
		if (m_MoonTextures.empty())
		{
			// make a list of planet textures
			std::string directoryPath = "../../../../_media/_assets/_textures/_demo/_moons/";
			std::string extension = ".png";
			for (const auto& entry : fs::directory_iterator(directoryPath))
			{
				if (entry.path().extension() == extension)
				{
					m_MoonTextures.push_back(entry.path().string());
				}
			}
		}

		KLS_Mesh* Mesh = Level->getDriver()->getResourceManager()->getMesh("../../../../_media/_assets/_models/_default/sphere.obj", KLSVT3D);
		std::string filename = m_MoonTextures[getRandomInt(0, static_cast<int>(m_MoonTextures.size()) - 1)];
		std::string bumpfilename = addTextToFilename(filename, "normal");
		bumpfilename = addFolderToFilename(bumpfilename, "bumpmaps/");

			Mesh->getMaterial(0).Texture_Diffuse[0] = filename;
			Mesh->getMaterial(0).Texture_Diffuse[1] = bumpfilename;
			Mesh->getMaterial(0).MaterialType = KLSMT_BUMPMAP;
		KLS_Entity e = Level->getECS()->createEntity(Level->getUniqueId(), name, transform);
		e.addComponent<KLS_COMPONENT_MESH>(Mesh);
		//e.addComponent<KLS_COMPONENT_ROTATE>(glm::vec3(0.0f, .1f, 0.0f));
		if (parent.getEntity() != entt::null) parent.addChild(e);
		return e;
	}


} // end namespace