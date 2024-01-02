#include "KLS_ResourceManager.h"
#include "KLS_Logger.h"
#include "_KLS_Driver.h"
#include "KLS_Texture.h"
#include "KLS_Texture_Cubemap.h"
#include "KLS_Colors.h"

// unclutter the global namespace
namespace KLS
{
	// bind a texture for shader use
	bool KLS_ResourceManager::bindTexture(int slot, std::string name)
	{
		KLS_Texture* t = getTexture(name, 0);
		if (t) { t->bind(slot); return KLS_SUCCESS; }
		return KLS_FAILURE;
	}

	// bind a cubemap for shader use
	bool KLS_ResourceManager::bindCubemap(int slot, std::string name)
	{
		KLS_Texture* t = getCubemap(name);
		if (t) { t->bind(slot); return KLS_SUCCESS; }
		return KLS_FAILURE;
	}

	// imgui helper function
	ImTextureID KLS_ResourceManager::getTextureId(std::string name)
	{
		KLS_Texture* t = getTexture(name,0);
		if (t)
		{
			uint64_t id = t->getRendererID();
			return reinterpret_cast<ImTextureID>(id);
		}
		return 0;
	}

	void KLS_ResourceManager::logTextures()
	{
		KLS_WARN("   Used Textures");
		for (auto& entry : m_Textures)
			KLS_TRACE("{}", entry.first.c_str());
	}

	void KLS_ResourceManager::logFailedTextures()
	{
		KLS_ERROR("   Failed Textures");
		for (int x = 0; x < m_FailedTextures.size(); x++)
			KLS_TRACE("{}", m_FailedTextures[x].c_str());
	}

	// add a texture to the list
	void KLS_ResourceManager::addTexture(KLS_Texture* texture)
	{
		if (!texture)
		{
			KLS_WARN("attempting to add an invalid texture");
			return;
		}

		// log this activity
		KLS_TRACE("KLS_ResourceManager::addTexture({})", texture->getName().c_str());

		// if the texture is already in the map then let the user know and then bail
		auto it = m_Textures.find(texture->getName());
		if (it != m_Textures.end())
		{
			KLS_WARN("texture {} already exists", texture->getName().c_str());
			return;
		}

		// add the texture to the map
		m_Textures.insert(std::make_pair(texture->getName(), texture));
	}

	// remove a texture from the list
	void KLS_ResourceManager::removeTexture(std::string name)
	{
		// log this activity
		KLS_TRACE("KLS_ResourceManager::removeTexture({})", name.c_str());

		// find the texture
		auto it = m_Textures.find(name);

		// if it is in the map then delete it
		if (it != m_Textures.end())
		{
			delete it->second;
			m_Textures.erase(it);
		}
		else
		{
			KLS_WARN("attempting to remove non existent texture {}", name.c_str());
		}
	}

	// destroy all of the textures
	void KLS_ResourceManager::removeAllTextures()
	{
		// log this activity
		KLS_TRACE("KLS_ResourceManager::removeAllTextures()");

		// delete each entry and then clear the map
		for (auto& entry : m_Textures) { delete entry.second; }
		m_Textures.clear();
	}

	// rename a texture
	void KLS_ResourceManager::renameTexture(std::string oldname, std::string newname)
	{
		// Log this activity
		KLS_TRACE("KLS_ResourceManager::renameTexture({}, {})", oldname.c_str(), newname.c_str());

		// Find the range of elements with the old name
		auto range = m_Textures.equal_range(oldname);

		// Check if any elements with the old name exist in the multimap
		if (range.first != m_Textures.end()) 
		{
			// Iterate through the range to find and erase the specific element
			for (auto it = range.first; it != range.second; ++it) 
			{
				if (it->second->getName() == oldname) 
				{ 
					KLS_Texture* texturePtr = it->second;

					// Erase the current element
					m_Textures.erase(it);

					// Insert a new entry with the new name and the same Texture pointer
					m_Textures.emplace(newname, texturePtr);

					// Break after renaming the first occurrence if that's the desired behavior
					break;
				}
			}
		}
		else 
		{
			KLS_WARN("Texture named {} does not exist", oldname.c_str());
		}
	}

	bool KLS_ResourceManager::isInFailedTextureList(const std::string& name) const
	{
		// Use std::find to check if the string is in the vector
		auto it = std::find(m_FailedTextures.begin(), m_FailedTextures.end(), name);
		return (it != m_FailedTextures.end());
	}

	KLS_Texture* KLS_ResourceManager::getTexture(std::string name, int flip)
	{
		// if we already failed laoding this texture then return the default texture
		// to stop the logging text from scrolling each time
		if (!isInFailedTextureList(name))
		{
			// if the texture is already in the map then just return the texture
			auto it = m_Textures.find(name);
			if (it != m_Textures.end()) return (it->second);

			// attempt to create it, it is succeeeds then add it to the list and return the new texture
			KLS_Texture* t = createTexture(name, flip);
			if (t) return t;

			// add it to the failed list to prevent the log from spamming the screen
			m_FailedTextures.push_back(name);
			KLS_WARN("using default texture for {}", name.c_str());
		}

		// otherwise, this is our first time to fail this texture
		// so log the event for the user and then return the default texture
		auto it2 = m_Textures.find(KLS_DEFAULT_TEXTURE_NAME);
		if (it2 != m_Textures.end()) return (it2->second);

		// the default texture doesnt exists so create it and then return it
		KLS_WARN("KLS_ResourceManager::getTexture() creating default texture!");
		KLS_Texture* defaulttexture = createTexture(KLS_DEFAULT_TEXTURE_NAME,flip, 64, 64, KLSTF_RGBA);
		if (defaulttexture)
		{
			defaulttexture->fill(KLS_CYAN);
			for (int x = 0; x < defaulttexture->getWidth(); x++)
				defaulttexture->setPixel(x, defaulttexture->getHeight() / 2, KLS_MAGENTA);
			for (int y = 0; y < defaulttexture->getHeight(); y++)
				defaulttexture->setPixel(defaulttexture->getWidth() / 2, y, KLS_MAGENTA);
			return defaulttexture;
		}

		// something went terribly wrong here, we should at least have a default texture to return?
		KLS_WARN("no default texture found");
		return nullptr;
	}


	// Cubemap
	void KLS_ResourceManager::addCubemap(KLS_Texture_Cubemap* texture)
	{
		if (!texture)
		{
			KLS_WARN("attempting to add an invalid Cubemap");
			return;
		}

		// log this activity
		KLS_TRACE("KLS_ResourceManager::removeAddCubemap({})", texture->getName().c_str());

		// if the texture is already in the map then let the user know and then bail
		auto it = m_Cubemaps.find(texture->getName());
		if (it != m_Cubemaps.end())
		{
			KLS_WARN("Cubemap {} already exists", texture->getName().c_str());
			return;
		}

		// add the texture to the map
		m_Cubemaps.insert(std::make_pair(texture->getName(), texture));
	}

	void KLS_ResourceManager::removeAllCubemaps()
	{
		// log this activity
		KLS_TRACE("KLS_ResourceManager::removeAllCubemaps()");

		// delete each entry and then clear the map
		for (auto& entry : m_Cubemaps) { delete entry.second; }
		m_Cubemaps.clear();
	}

	void KLS_ResourceManager::removeCubemap(std::string name)
	{
		// log this activity
		KLS_TRACE("KLS_ResourceManager::removeCubemap({})", name.c_str());

		// find the texture
		auto it = m_Cubemaps.find(name);

		// if it is in the map then delete it
		if (it != m_Cubemaps.end())
		{
			delete it->second;
			m_Cubemaps.erase(it);
		}
		else
		{
			KLS_WARN("attempting to remove non existent Cubemap {}", name.c_str());
		}
	}

	KLS_Texture_Cubemap* KLS_ResourceManager::getCubemap(std::string name)
	{
		// if the texture is already in the map then just return the texture
		auto it = m_Cubemaps.find(name);
		if (it != m_Cubemaps.end()) return (it->second);

		// something went wrong here
		KLS_WARN("KLS_ResourceManager::getCubemap({}) failed", name.c_str());
		return nullptr;
	}

} // end namespace
