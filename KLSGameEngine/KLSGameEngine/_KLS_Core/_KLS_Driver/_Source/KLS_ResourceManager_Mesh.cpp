// include the needed header files
#include "KLS_ResourceManager.h"
#include "KLS_Logger.h"
#include "_KLS_Driver.h"
#include "KLS_Mesh.h"
#include "KLS_AnimatedMesh.h"
#include "KLS_MeshBuffer.h"
#include "KLS_File.h"
#include "KLS_AssimpHelpers.h"

// unclutter the global namespace
namespace KLS
{
	KLS_Mesh* KLS_ResourceManager::createMesh(KLS_VERTEX_TYPE type, std::string name, std::vector<float> vertices, std::vector<uint32_t> indices)
	{
		KLS_Mesh* mesh = new KLS_Mesh(getDriver(), type, getVertexBufferLayout(type, false), vertices, indices);
		m_Meshes.emplace(name, mesh);
		mesh->setName(name);
		return mesh;
	}

	KLS_Mesh* KLS_ResourceManager::getMesh(std::string name, KLS_VERTEX_TYPE type)
	{
		//auto it = m_Meshes.find(name);
		//if (it != m_Meshes.end()) return it->second;
		
		KLS_Mesh* mesh = createMesh(type, name);
		if (mesh)
		{
			m_Meshes.emplace(name, mesh);
			mesh->setName(name);
			return mesh;
		}

		// something went wrong
		return nullptr;
	}

	void KLS_ResourceManager::removeAllMeshes()
	{
		// log this activity
		KLS_TRACE("KLS_ResourceManager::removeAllMeshes()");

		// delete each entry and then clear the map
		for (auto& entry : m_Meshes) { delete entry.second; }
		m_Meshes.clear();
	}

	void KLS_ResourceManager::removeMesh(KLS_Mesh* mesh)
	{
		// Iterate through the map to find the key associated with the provided mesh pointer
		for (auto it = m_Meshes.begin(); it != m_Meshes.end(); ++it) 
		{
			if (it->second == mesh) 
			{ // Check if the pointer matches
				delete it->second; // Delete the pointed object if needed
				m_Meshes.erase(it); // Remove the item from the map
				break; // Exit loop once the item is erased
			}
		}
	}

	KLS_Mesh* KLS_ResourceManager::createMesh(KLS_VERTEX_TYPE type, std::string name)
	{
		KLS_Mesh* mesh = nullptr;
		if (name == KLS_DEFAULT_STRING) return nullptr;

		// try to load it
		mesh = KLS_AssimpHelpers::loadMeshFromFile(getDriver(), type, name);

		// if we loaded it then add it to the list and return it
		if (mesh) return mesh;

		// warn the user we couldnt load the mesh file
		KLS_WARN("unable to load mesh file ({})", name.c_str());

		// something went wrong
		return nullptr;
	}


} // end namespace
