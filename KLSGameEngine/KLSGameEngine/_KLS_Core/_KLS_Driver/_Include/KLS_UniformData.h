// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Logger.h"
#include "KLS_Event.h"
#include "KLS_KeyCodes.h"

// unclutter the global namespace
namespace KLS
{
	struct KLS_UniformData
	{
		glm::mat4 ProjMatrix;
		glm::mat4 ViewMatrix;
		glm::vec4 ClipPlane;
		glm::vec4 CameraPos;
		glm::vec4 AmbientColor;
		glm::vec4 Misc; // [0]AmbientIntensity, [1] numberoflights, [2] totaltime
	};

	struct KLS_LightData
	{
		glm::vec4 color = KLS_WHITE;        // The color of the light (RGB values)
		glm::vec4 position;
		glm::vec4 intensity = glm::vec4(1);
		glm::vec4 attenuation = glm::vec4(0.01, .08, .04, 1000);
		float cameraDistance;
	};

	struct KLS_UniformDataLights
	{
		std::array<glm::vec4, KLS_MAX_LIGHTS> lightColor;
		std::array<glm::vec4, KLS_MAX_LIGHTS> lightPosition;
		std::array<glm::vec4, KLS_MAX_LIGHTS> lightIntensity;
		std::array<glm::vec4, KLS_MAX_LIGHTS> lightAttenuation;

		void clear()
		{
			lightColor.fill(glm::vec4(0, 0, 0, 1.0f));
			lightPosition.fill(glm::vec4(0, 0, 0, 0));
			lightAttenuation.fill(glm::vec4(0, 0, 0, 0));
			lightIntensity.fill(glm::vec4(1, 1, 1, 1));
		}

		// Constructor to initialize the arrays
		KLS_UniformDataLights()
		{
			lightColor.fill(glm::vec4(0, 0, 0, 1.0f));
			lightPosition.fill(glm::vec4(0, 0, 0, 0));
			lightAttenuation.fill(glm::vec4(0, 0, 0, 0));
			lightIntensity.fill(glm::vec4(1, 1, 1, 1));
		}

		// Copy constructor
		KLS_UniformDataLights(const KLS_UniformDataLights& other)
		{
			lightColor = other.lightColor;
			lightPosition = other.lightPosition;
			lightAttenuation = other.lightAttenuation;
			lightIntensity = other.lightIntensity;
		}

		// Assignment operator
		KLS_UniformDataLights& operator=(const KLS_UniformDataLights& other)
		{
			if (this != &other) // Check for self-assignment
			{
				lightColor = other.lightColor;
				lightPosition = other.lightPosition;
				lightAttenuation = other.lightAttenuation;
				lightIntensity = other.lightIntensity;
			}
			return *this; // Return a reference to the modified object
		}

		static size_t getSize()
		{
			return sizeof(KLS_UniformDataLights) +
				sizeof(glm::vec4) * KLS_MAX_LIGHTS +
				sizeof(glm::vec4) * KLS_MAX_LIGHTS +
				sizeof(glm::vec4) * KLS_MAX_LIGHTS +
				sizeof(glm::vec4) * KLS_MAX_LIGHTS;
		}
	};

} // end namespace
