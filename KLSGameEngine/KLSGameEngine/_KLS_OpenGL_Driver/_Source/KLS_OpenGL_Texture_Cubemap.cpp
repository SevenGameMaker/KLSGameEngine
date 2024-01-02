// include the needed header files
#include "KLS_OpenGL_Texture_Cubemap.h"
#include "KLS_OpenGL_Texture.h"
#include "KLS_Logger.h"
#include "_KLS_Driver.h"
#include <glad/gl.h>
#include "KLS_ResourceManager.h"

// unclutter the global namespace
namespace KLS
{
	// convert our internal format to an opengl format
	GLenum KLSTextureFormatToOpenGLGpuTextureFormat(KLS_TextureFormat format)
	{
		switch (format)
		{
		case KLSTF_RG16F: return GL_RG16F; break;
		case KLSTF_RGB: return GL_RGB; break;
		case KLSTF_RGBA: return GL_RGBA; break;
		}
		return KLS_TextureFormat::KLSTF_NONE;
	}

	// class constructor / destructor
	KLS_OpenGL_Texture_Cubemap::KLS_OpenGL_Texture_Cubemap() : KLS_Texture_Cubemap()
	{
	}

	KLS_OpenGL_Texture_Cubemap::~KLS_OpenGL_Texture_Cubemap()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	bool KLS_OpenGL_Texture_Cubemap::create(KLS_Driver* driver, std::string name, int flip,
		std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back)
	{
		KLS_CHECK(KLS_Texture_Cubemap::create(driver, name, flip,
			right, left, top, bottom, front, back), "base class cubemap creation failed", KLS_FAILURE);

		// make sure we can load all of the textures
		KLS_Texture* tright = getDriver()->getResourceManager()->getTexture(right, 0);
		KLS_Texture* tleft = getDriver()->getResourceManager()->getTexture(left, 0);
		KLS_Texture* ttop = getDriver()->getResourceManager()->getTexture(top, 0);
		KLS_Texture* tbottom = getDriver()->getResourceManager()->getTexture(bottom, 0);
		KLS_Texture* tfront = getDriver()->getResourceManager()->getTexture(front, 0);
		KLS_Texture* tback = getDriver()->getResourceManager()->getTexture(back, 0);
		if (!tright || !tleft || !ttop || !tbottom || !tfront || !tback)
		{
			KLS_WARN("cubemap creation failed. unable to load all images");
			return KLS_FAILURE;
		}
		std::vector<KLS_Texture*> textures;
		textures.push_back(tright);
		textures.push_back(tleft);
		textures.push_back(ttop);
		textures.push_back(tbottom);
		textures.push_back(tfront);
		textures.push_back(tback);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// Load image data into the cubemap faces
		for (unsigned int i = 0; i < 6; i++)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, // Target face
				0,                                   // Level of detail
//				KLSTextureFormatToOpenGLGpuTextureFormat(textures[i]->getFormat()),                             // Internal format
				textures[i]->getInternalFormat(),                             // Internal format
				textures[i]->getWidth(),             // Width of the texture
				textures[i]->getHeight(),            // Height of the texture
				0,                                   // Border (must be 0)
//				KLSTextureFormatToOpenGLGpuTextureFormat(textures[i]->getFormat()),                             // Internal format
				textures[i]->getGPUFormat(),                             // Internal format
				GL_UNSIGNED_BYTE,                    // Data type of the image data
				textures[i]->getCharData()           // Image data for the current face
			);
		}
		return KLS_SUCCESS;
	}


	void KLS_OpenGL_Texture_Cubemap::bind(uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
	}

	void KLS_OpenGL_Texture_Cubemap::unbind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

} // end namespace