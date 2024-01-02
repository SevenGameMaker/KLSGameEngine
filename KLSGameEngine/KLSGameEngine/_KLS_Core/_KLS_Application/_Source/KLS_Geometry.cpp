#include "KLS_Geometry.h"

namespace KLS
{
	std::vector<uint32_t> KLS_API createIndices_Quad2D()
	{
		return std::vector<uint32_t> { 0, 1, 2, 0, 2, 3 };
	}

	std::vector<float> KLS_API createVertices_Quad2D(float size)
	{
		return std::vector<float>
		{
			-size, -size, 0.0f, 0.0f,
			size, -size, 1.0f, 0.0f,
			size, size, 1.0f, 1.0f,
			-size, size, 0.0f, 1.0f,
		};
	}

    std::vector<uint32_t> createIndices_Cube()
    {
        std::vector<unsigned int> indices{
            // Front face
            0, 1, 2,
            2, 3, 0,

            // Back face
            4, 5, 6,
            6, 7, 4,

            // Left face
            8, 9, 10,
            10, 11, 8,

            // Right face
            12, 13, 14,
            14, 15, 12,

            // Top face
            16, 17, 18,
            18, 19, 16,

            // Bottom face
            20, 21, 22,
            22, 23, 20
        };

        return indices;
    }

    std::vector<float> createVertices_Cube(glm::vec3 minimum, glm::vec3 maximum)
    {
        std::vector<float> vertices{
            // Front face
            minimum.x, minimum.y, minimum.z,0,0,0,0,0,
            maximum.x, minimum.y, minimum.z,0,0,0,0,0,
            maximum.x, maximum.y, minimum.z,0,0,0,0,0,
            minimum.x, maximum.y, minimum.z,0,0,0,0,0,
            maximum.x, minimum.y, maximum.z,0,0,0,0,0,
            minimum.x, minimum.y, maximum.z,0,0,0,0,0,
            minimum.x, maximum.y, maximum.z,0,0,0,0,0,
            maximum.x, maximum.y, maximum.z,0,0,0,0,0,
            minimum.x, minimum.y, maximum.z,0,0,0,0,0,
            minimum.x, minimum.y, minimum.z,0,0,0,0,0,
            minimum.x, maximum.y, minimum.z,0,0,0,0,0,
            minimum.x, maximum.y, maximum.z,0,0,0,0,0,
            maximum.x, minimum.y, minimum.z,0,0,0,0,0,
            maximum.x, minimum.y, maximum.z,0,0,0,0,0,
            maximum.x, maximum.y, maximum.z,0,0,0,0,0,
            maximum.x, maximum.y, minimum.z,0,0,0,0,0,
            minimum.x, maximum.y, minimum.z,0,0,0,0,0,
            maximum.x, maximum.y, minimum.z,0,0,0,0,0,
            maximum.x, maximum.y, maximum.z,0,0,0,0,0,
            minimum.x, maximum.y, maximum.z,0,0,0,0,0,
            maximum.x, minimum.y, minimum.z,0,0,0,0,0,
            minimum.x, minimum.y, minimum.z,0,0,0,0,0,
            minimum.x, minimum.y, maximum.z,0,0,0,0,0,
            maximum.x, minimum.y, maximum.z,0,0,0,0,0
        };

        return vertices;
    }

    // Function to create indices for a 3D grid
    std::vector<uint32_t> createIndices_Grid(int gridSize) 
    {
        std::vector<uint32_t> indices;

        for (int i = 0; i < gridSize * 2; ++i) {
            for (int j = 0; j < gridSize * 2; ++j) {
                int row1 = i * (gridSize * 2 + 1);
                int row2 = (i + 1) * (gridSize * 2 + 1);

                indices.push_back(row1 + j);
                indices.push_back(row1 + j + 1);
                indices.push_back(row2 + j + 1);

                indices.push_back(row1 + j);
                indices.push_back(row2 + j + 1);
                indices.push_back(row2 + j);
            }
        }

        return indices;
    }

    // Function to create vertices for a 3D grid
    std::vector<float> createVertices_Grid(int gridSize, float spacing) 
    {
        std::vector<float> vertices;

        // Generate vertices, normals, texture coordinates
        for (int i = -gridSize; i <= gridSize; ++i) {
            for (int j = -gridSize; j <= gridSize; ++j) {
                float x = j * spacing;
                float z = i * spacing;

                // Vertices
                vertices.push_back(x);
                vertices.push_back(0.0f);
                vertices.push_back(z);
                vertices.push_back(0.0f);
                vertices.push_back(1.0f);
                vertices.push_back(0.0f);
                vertices.push_back((float)j / (float)(2 * gridSize));
                vertices.push_back((float)i / (float)(2 * gridSize));
            }
        }
        return vertices;
    }
} // end namespace