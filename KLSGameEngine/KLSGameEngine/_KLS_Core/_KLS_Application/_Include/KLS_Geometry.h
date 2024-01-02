// parse this file only once
#pragma once

#include "_KLS_Defines.h"
#include "_KLS_Includes.h"

// unclutter the global namespace
namespace KLS
{
	// a few geometry related methods for ease of use
	// so we dont have to create vertices / indices in the code
	std::vector<uint32_t> KLS_API createIndices_Quad2D();
	std::vector<float> KLS_API createVertices_Quad2D(float size);

	std::vector<uint32_t> KLS_API createIndices_Cube();
	std::vector<float> createVertices_Cube(glm::vec3 mininum, glm::vec3 maximum);

	std::vector<uint32_t> KLS_API createIndices_Grid(int gridSize);
	std::vector<float> KLS_API createVertices_Grid(int gridSize, float spacing);

} // end namespace
