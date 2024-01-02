// include the required header files
#include "KLS_AssimpHelpers.h"
#include "_KLS_Driver.h"
#include "KLS_Mesh.h"
#include "KLS_AnimatedMesh.h"
#include "KLS_MeshBuffer.h"
#include "KLS_Material.h"
#include "KLS_File.h"

// unclutter the global namespace
namespace KLS
{
	glm::mat4 KLS_AssimpHelpers::ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
	{
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}

	glm::vec3 KLS_AssimpHelpers::GetGLMVec(const aiVector3D& vec)
	{
		return glm::vec3(vec.x, vec.y, vec.z);
	}

	glm::quat KLS_AssimpHelpers::GetGLMQuat(const aiQuaternion& pOrientation)
	{
		return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
	}

	std::vector<std::string> KLS_AssimpHelpers::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string path)
	{
		std::vector<std::string> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			textures.push_back(path + std::string(str.C_Str()));
		}
		return textures;
	}

	KLS_Mesh* KLS_AssimpHelpers::loadMeshFromFile(KLS_Driver* driver, KLS_VERTEX_TYPE type, std::string filename)
	{
		KLS_INFO("KLS_ResourceManager::loadMeshFromFile({})", filename.c_str());

		std::string path = extractFilePath(filename);

		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename,
			aiProcess_Triangulate
			| aiProcess_GenNormals
		);

		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			KLS_WARN("ERROR::ASSIMP:: {}", importer.GetErrorString());
			return nullptr;
		}

		// retrieve the directory path of the filepath
		//directory = path.substr(0, path.find_last_of('/'));

		KLS_Mesh* mesh = new KLS_Mesh(driver);

		// process ASSIMP's root node recursively
		KLS_AssimpHelpers::processNode(driver, type, mesh, scene->mRootNode, scene, path);

		return mesh;
	}

	void KLS_AssimpHelpers::processMesh(KLS_Driver* driver, KLS_VERTEX_TYPE type, KLS_Mesh* klsmesh, aiMesh* mesh, const aiScene* scene, std::string path)
	{
		std::vector<float> vertexData;
		std::vector<uint32_t> indexData;

		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			switch (type)
			{
			case KLSVT3D:
			{
				// positions
				vertexData.push_back(mesh->mVertices[i].x);
				vertexData.push_back(mesh->mVertices[i].y);
				vertexData.push_back(mesh->mVertices[i].z);

				// texture coordinates
				if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					vertexData.push_back(mesh->mTextureCoords[0][i].x);
					vertexData.push_back(mesh->mTextureCoords[0][i].y);
				}
				else
				{
					vertexData.push_back(0);
					vertexData.push_back(1);
				}

				// normals
				if (mesh->HasNormals())
				{
					vertexData.push_back(mesh->mNormals[i].x);
					vertexData.push_back(mesh->mNormals[i].y);
					vertexData.push_back(mesh->mNormals[i].z);
				}
				else
				{
					vertexData.push_back(0.0f);
					vertexData.push_back(1.0f);
					vertexData.push_back(0.0f);
				}
			} break;
			}
		}

		// now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indexData.push_back(face.mIndices[j]);
		}

		KLS_MeshBuffer* mb = klsmesh->addMeshBuffer(new KLS_MeshBuffer(driver, type, getVertexBufferLayout(type, false), vertexData, indexData));

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<std::string> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, path);
			std::vector<std::string> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, path);
			std::vector<std::string> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, path);
			std::vector<std::string> displacementMaps = loadMaterialTextures(material, aiTextureType_DISPLACEMENT, path);

			for (uint32_t x = 0; x < diffuseMaps.size(); x++) { mb->getMaterial().Texture_Diffuse[x] = diffuseMaps[x]; }
			for (uint32_t x = 0; x < specularMaps.size(); x++) { mb->getMaterial().Texture_Specular[x] = specularMaps[x]; }
			for (uint32_t x = 0; x < normalMaps.size(); x++) { mb->getMaterial().Texture_Normal[x] = normalMaps[x]; }
			for (uint32_t x = 0; x < displacementMaps.size(); x++) { mb->getMaterial().Texture_Displacement[x] = displacementMaps[x]; }
		}
	}

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void KLS_AssimpHelpers::processNode(KLS_Driver* driver, KLS_VERTEX_TYPE type, KLS_Mesh* klsmesh, aiNode* node, const aiScene* scene, std::string path)
	{
		KLS_INFO("  numMeshes {}", node->mNumMeshes);

		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(driver, type, klsmesh, mesh, scene, path);
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(driver, type, klsmesh, node->mChildren[i], scene, path);
		}

	}

	void KLS_AssimpHelpers::SetVertexBoneDataToDefault(Vertex& vertex)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			vertex.m_BoneIDs[i] = -1;
			vertex.m_Weights[i] = 0.0f;
		}
	}

	void KLS_AssimpHelpers::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
		{
			if (vertex.m_BoneIDs[i] < 0)
			{
				vertex.m_Weights[i] = weight;
				vertex.m_BoneIDs[i] = (float)boneID;
				break;
			}
		}
	}

	void KLS_AssimpHelpers::ExtractBoneWeightForVertices(KLS_AnimatedMesh* klsmesh, std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
	{
		auto& boneInfoMap = klsmesh->m_BoneInfoMap;
		int& boneCount = klsmesh->m_BoneCounter;

		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
		{
			int boneID = -1;
			std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				KLS_BoneInfo newBoneInfo;
				newBoneInfo.id = boneCount;
				newBoneInfo.offset = KLS_AssimpHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
				boneInfoMap[boneName] = newBoneInfo;
				boneID = boneCount;
				boneCount++;
			}
			else
			{
				boneID = boneInfoMap[boneName].id;
			}
			assert(boneID != -1);
			auto weights = mesh->mBones[boneIndex]->mWeights;
			int numWeights = mesh->mBones[boneIndex]->mNumWeights;

			for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
			{
				int vertexId = weights[weightIndex].mVertexId;
				float weight = weights[weightIndex].mWeight;
				assert(vertexId <= vertices.size());
				SetVertexBoneData(vertices[vertexId], boneID, weight);
			}
		}
	}

	std::vector<std::string> KLS_AssimpHelpers::loadAnimatedMaterialTextures(aiMaterial* mat, aiTextureType type, std::string path)
	{
		std::vector<std::string> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			textures.push_back(path + std::string(str.C_Str()));
		}
		return textures;
	}

	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	KLS_AnimatedMesh* KLS_AssimpHelpers::loadAnimatedMeshFromFile(KLS_Driver* driver, KLS_VERTEX_TYPE type, std::string filename)
	{
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			KLS_WARN(" ... {}", importer.GetErrorString());
			if (!scene) { KLS_WARN(" ... no scene"); return nullptr; }
			if (!scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) KLS_WARN(" ... scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE");
			if (!!scene->mRootNode) KLS_WARN(" ... !scene->mRootNode");
			return nullptr;
		}

		KLS_AnimatedMesh* klsmesh = new KLS_AnimatedMesh(driver);

		std::string path = extractFilePath(filename);

		// process ASSIMP's root node recursively
		KLS_AssimpHelpers::processAnimatedNode(driver, type, klsmesh, scene->mRootNode, scene, path);

		return klsmesh;
	}

	void KLS_AssimpHelpers::processAnimatedMesh(KLS_Driver* driver, KLS_VERTEX_TYPE type, KLS_AnimatedMesh* klsmesh, aiMesh* mesh, const aiScene* scene, std::string path)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indexData;
		std::vector<std::string> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			SetVertexBoneDataToDefault(vertex);
			vertex.Position = KLS_AssimpHelpers::GetGLMVec(mesh->mVertices[i]);
			vertex.Normal = KLS_AssimpHelpers::GetGLMVec(mesh->mNormals[i]);

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indexData.push_back(face.mIndices[j]);
		}
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//return Mesh(vertices, indices, textures);
		ExtractBoneWeightForVertices(klsmesh, vertices, mesh, scene);

		std::vector<float> vertexData;
		for (int x = 0; x < vertices.size(); x++)
		{
			vertexData.push_back(vertices[x].Position.x);
			vertexData.push_back(vertices[x].Position.y);
			vertexData.push_back(vertices[x].Position.z);

			vertexData.push_back(vertices[x].TexCoords.x);
			vertexData.push_back(vertices[x].TexCoords.y);

			vertexData.push_back(vertices[x].Normal.x);
			vertexData.push_back(vertices[x].Normal.y);
			vertexData.push_back(vertices[x].Normal.z);

			for (int bi = 0; bi < MAX_BONE_INFLUENCE; bi++)
				vertexData.push_back(vertices[x].m_BoneIDs[bi]);
			for (int bw = 0; bw < MAX_BONE_INFLUENCE; bw++)
				vertexData.push_back(vertices[x].m_Weights[bw]);
		}
		KLS_MeshBuffer* mb = klsmesh->addMeshBuffer(new KLS_MeshBuffer(driver, type, getVertexBufferLayout(type, false), vertexData, indexData));

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<std::string> diffuseMaps = loadAnimatedMaterialTextures(material, aiTextureType_DIFFUSE, path);
			std::vector<std::string> specularMaps = loadAnimatedMaterialTextures(material, aiTextureType_SPECULAR, path);
			std::vector<std::string> normalMaps = loadAnimatedMaterialTextures(material, aiTextureType_NORMALS, path);
			std::vector<std::string> displacementMaps = loadAnimatedMaterialTextures(material, aiTextureType_DISPLACEMENT, path);

			for (uint32_t x = 0; x < diffuseMaps.size(); x++) { mb->getMaterial().Texture_Diffuse[x] = diffuseMaps[x]; }
			for (uint32_t x = 0; x < specularMaps.size(); x++) { mb->getMaterial().Texture_Specular[x] = specularMaps[x]; }
			for (uint32_t x = 0; x < normalMaps.size(); x++) { mb->getMaterial().Texture_Normal[x] = normalMaps[x]; }
			for (uint32_t x = 0; x < displacementMaps.size(); x++) { mb->getMaterial().Texture_Displacement[x] = displacementMaps[x]; }
		}

	}

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void KLS_AssimpHelpers::processAnimatedNode(KLS_Driver* driver, KLS_VERTEX_TYPE type, KLS_AnimatedMesh* klsmesh, aiNode* node, const aiScene* scene, std::string path)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			processAnimatedMesh(driver, type, klsmesh, mesh, scene, path);
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processAnimatedNode(driver, type, klsmesh, node->mChildren[i], scene, path);
		}

	}

} // end namespace
