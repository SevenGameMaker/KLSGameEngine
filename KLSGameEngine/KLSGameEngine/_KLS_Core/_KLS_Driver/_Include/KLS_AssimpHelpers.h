// parse this file only once
#pragma once

#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"


#include<assimp/quaternion.h>
#include<assimp/vector3.h>
#include<assimp/matrix4x4.h>
#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// unclutter the global namespace
namespace KLS
{
	// forward declarations
	typedef class KLS_Driver KLS_Driver;
	typedef class KLS_Mesh KLS_Mesh;
	typedef class KLS_AnimatedMesh KLS_AbimatedMesh;
	typedef class KLS_MeshBuffer KLS_MeshBuffer;

	// this structure is only for helping us load an animated mesh 
	// (it could be done without it but this is easier)
	struct KLS_API Vertex 
	{
		// position
		glm::vec3 Position;
		// normal
		glm::vec3 Normal;
		// texCoords
		glm::vec2 TexCoords;
		//bone indexes which will influence this vertex
		float m_BoneIDs[MAX_BONE_INFLUENCE];
		//weights from each bone
		float m_Weights[MAX_BONE_INFLUENCE];
	};

	class KLS_API KLS_AssimpHelpers
	{
	public:
		// simple helper methods
		static glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from);
		static glm::vec3 GetGLMVec(const aiVector3D& vec);
		static glm::quat GetGLMQuat(const aiQuaternion& pOrientation);

		// load the assimp materials into the vector of strings to be consumed by calling class
		static std::vector<std::string> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string path);
		static std::vector <std::string> loadAnimatedMaterialTextures(aiMaterial* mat, aiTextureType type, std::string path);

		// load an unanimated mesh into a KLS_Mesh instance
		static KLS_Mesh* loadMeshFromFile(KLS_Driver* driver, KLS_VERTEX_TYPE type, std::string filename);
			static void processMesh(KLS_Driver* driver, KLS_VERTEX_TYPE type, KLS_Mesh* klsmesh, aiMesh* mesh, const aiScene* scene, std::string path);
			static void processNode(KLS_Driver* driver, KLS_VERTEX_TYPE type, KLS_Mesh* klsmesh, aiNode* node, const aiScene* scene, std::string path);

		// load an animated mesh into a KLS_Mesh instance
		static KLS_AnimatedMesh* loadAnimatedMeshFromFile(KLS_Driver* driver, KLS_VERTEX_TYPE type, std::string filename);
			static void SetVertexBoneDataToDefault(Vertex& vertex);
			static void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
			static void ExtractBoneWeightForVertices(KLS_AnimatedMesh* klsmesh, std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
			static void processAnimatedMesh(KLS_Driver* driver, KLS_VERTEX_TYPE type, KLS_AnimatedMesh* klsmesh, aiMesh* mesh, const aiScene* scene, std::string path);
			static void processAnimatedNode(KLS_Driver* driver, KLS_VERTEX_TYPE type, KLS_AnimatedMesh* klsmesh, aiNode* node, const aiScene* scene, std::string path);
	};

} // end namespace
