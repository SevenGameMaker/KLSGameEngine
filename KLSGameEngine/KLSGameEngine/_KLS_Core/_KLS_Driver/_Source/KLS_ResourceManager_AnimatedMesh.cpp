// include the needed header files
#include "KLS_ResourceManager.h"
#include "KLS_Logger.h"
#include "_KLS_Driver.h"
#include "KLS_Mesh.h"
#include "KLS_AnimatedMesh.h"
#include "KLS_MeshBuffer.h"
#include "KLS_File.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "KLS_AssimpHelpers.h"
#include "KLS_Animation.h"

// unclutter the global namespace
namespace KLS
{
	KLS_AnimatedMesh* KLS_ResourceManager::createAnimatedMesh(KLS_VERTEX_TYPE type, std::string name)
	{
		KLS_AnimatedMesh* mesh = nullptr;
		if (name == KLS_DEFAULT_STRING) return nullptr;

		// try to load it
		mesh = KLS_AssimpHelpers::loadAnimatedMeshFromFile(getDriver(), type, name);

		// if we loaded it then add it to the list and return it
		if (mesh)
		{
			m_Meshes.emplace(name, mesh);
			return mesh;
		}

		KLS_WARN("unable to load mesh file ({})", name.c_str());

		return nullptr;
	}




} // end namespace
