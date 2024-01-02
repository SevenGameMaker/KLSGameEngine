#include "KLS_ResourceManager.h"
#include "_KLS_Driver.h"
#include "KLS_Texture.h"
#include "KLS_Texture_Cubemap.h"

namespace KLS
{
	KLS_Texture* KLS_ResourceManager::createTexture(std::string filename, int flip)
	{
		// get a pointer to a new texture instance
		KLS_Texture* t = m_Driver->instantiateTexture();

		// attempt to create it, it is succeeeds then add it to the list and return the new texture
		if (t->create(m_Driver, filename, flip))
		{
			addTexture(t);
			return t;
		}

		// creation failed, delete the instance and return nullptr
		delete(t);
		return nullptr;
	}

	KLS_Texture* KLS_ResourceManager::createTexture(std::string name, int flip,
		int width, int height, KLS_TextureFormat format)
	{
		// get a pointer to a new texture instance
		KLS_Texture* t = m_Driver->instantiateTexture();

		// attempt to create it, it is succeeeds then return the new texture
		if (t->create(m_Driver, name, flip, width, height, format))
		{
			addTexture(t);
			return t;
		}

		// creation failed, delete the instance and return nullptr
		delete(t);
		return nullptr;
	}

	KLS_Texture_Cubemap* KLS_ResourceManager::createCubemap(std::string name, int flip,
		std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back)
	{
		// get a pointer to a new texture instance
		KLS_Texture_Cubemap* t = m_Driver->instantiateCubemap();

		// attempt to create it, it is succeeeds then return the new texture
		if (t->create(m_Driver, name, flip, right, left, top, bottom, front, back))
		{
			addCubemap(t);
			return t;
		}

		// creation failed, delete the instance and return nullptr
		delete(t);
		return nullptr;
	}

} // end namespace