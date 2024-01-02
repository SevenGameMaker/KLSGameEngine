#include "KLS_OpenGL_FrameBuffer.h"
#include "_KLS_Driver.h"
#include "KLS_Texture.h"
#include "KLS_ResourceManager.h"
#include "KLS_Application.h"
#include <glad/gl.h>

namespace KLS
{
	// do nothing constructor / destructor (use initialize, create and cleanup instead)
	KLS_OpenGL_FrameBuffer::KLS_OpenGL_FrameBuffer() : KLS_FrameBuffer()
	{
		// log this activity
		KLS_TRACE("KLS_OpenGL_FrameBuffer::KLS_OpenGL_FrameBuffer()");
	}
	
	KLS_OpenGL_FrameBuffer::~KLS_OpenGL_FrameBuffer()
	{
		// log this activity
		KLS_TRACE("KLS_OpenGL_FrameBuffer::~KLS_OpenGL_FrameBuffer()");
	}

	// set all variables to a known value
	void KLS_OpenGL_FrameBuffer::initialize()
	{ 
		// log this activity
		KLS_TRACE("KLS_OpenGL_FrameBuffer::initialize()");

		// call the base class
		KLS_FrameBuffer::initialize();
	}

	// dual creation allows for better error handling
	bool KLS_OpenGL_FrameBuffer::create(KLS_Driver* driver, std::string name, int width, int height, bool resize, bool drawbuffer)
	{ 
		// log this activity
		KLS_TRACE("KLS_OpenGL_FrameBuffer::create()");

		// call the base class
		KLS_CHECK(KLS_FrameBuffer::create(driver, name, width, height, resize, drawbuffer),"base class creation failed",KLS_FAILURE);

		// framebuffer configuration
		glGenFramebuffers(1, &m_RenderID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);

		// everything went fine
		return KLS_SUCCESS;
	}

	bool KLS_OpenGL_FrameBuffer::finalize()
	{
		// log this activity
		KLS_TRACE("KLS_OpenGL_FrameBuffer::finalize()");

		// call the base class
		KLS_CHECK(KLS_FrameBuffer::finalize(), "base class finalize failed", KLS_FAILURE);

		for (int x = 0; x < m_AttachmentInfo.size(); x++)
		{
			if (!m_AttachmentInfo[x].m_Depth)
			{
				KLS_Texture* texture = getDriver()->getResourceManager()->createTexture(m_AttachmentInfo[x].m_Name, false, m_Width, m_Height, m_AttachmentInfo[x].m_GPUFormat);
				m_ColorAttachments.push_back(texture);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + x, GL_TEXTURE_2D, m_ColorAttachments[x]->getRendererID(), 0);
			}
			else
			{
				if (m_AttachmentInfo[x].m_DepthOwner)
				{
					std::string name = m_Name + std::string("depthbuffer") + std::to_string(1);
					m_DepthAttachment = getDriver()->getResourceManager()->createTexture(name, false, m_Width, m_Height, KLSTF_DEPTH);
				}
				else
				{
					m_DepthAttachment = getDriver()->getResourceManager()->getTexture(m_AttachmentInfo[x].m_DepthName, false);
					//previous->bind(0);
				}
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment->getRendererID(), 0);
			}
		}

		std::vector<GLenum> buffers;
		for (int x = 0; x < m_ColorAttachments.size(); x++)
		{
			buffers.push_back(GL_COLOR_ATTACHMENT0 + x);
		}

		if (m_DrawBuffer)
		glDrawBuffers((GLsizei)m_ColorAttachments.size(), &buffers[0]);
		else
		glDrawBuffer(GL_NONE);

		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			KLS_ERROR("frame buffer creation failed");
			return KLS_FAILURE;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// everything went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memroy mess we made
	bool KLS_OpenGL_FrameBuffer::cleanup()
	{ 
		// log this activity
		KLS_TRACE("KLS_OpenGL_FrameBuffer::cleanup()");

		glDeleteFramebuffers(1, &m_RenderID);
		if (m_DepthAttachment)
		{
			if (m_DepthOwner)
				getDriver()->getResourceManager()->removeTexture(m_DepthAttachment->getName());
			m_DepthAttachment = nullptr;
		}

		for (int x = 0; x < m_ColorAttachments.size(); x++)
		{
			getDriver()->getResourceManager()->removeTexture(m_ColorAttachments[x]->getName());
		}
		m_ColorAttachments.clear();


		// call the base class
		return KLS_FrameBuffer::cleanup();
	}

	void KLS_OpenGL_FrameBuffer::bindForWrite()
	{
		// call the base class
		KLS_FrameBuffer::bindForWrite();

		// bind to framebuffer and draw scene as we normally would to color texture 
		glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);
		glViewport(0, 0, static_cast<int>(m_Width), static_cast<int>(m_Height));
	}

	void KLS_OpenGL_FrameBuffer::bindForRead(uint32_t slot)
	{
		// call the base class
		KLS_FrameBuffer::bindForRead(slot);
		for (int x = 0; x < m_ColorAttachments.size(); x++)
			m_ColorAttachments[x]->bind(slot + x);
	}

	void KLS_OpenGL_FrameBuffer::unbind()
	{
		// call the base class
		KLS_FrameBuffer::unbind();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void KLS_OpenGL_FrameBuffer::addAttachmentColor(std::string name, KLS_TextureFormat gpuformat)
	{
		KLS_FrameBuffer::addAttachmentColor(name, gpuformat);
	}

	void KLS_OpenGL_FrameBuffer::addAttachmentDepth(KLS_Texture* previous)
	{
		KLS_FrameBuffer::addAttachmentDepth(previous);
	}

	void KLS_OpenGL_FrameBuffer::addAttachmentDepth(std::string name, KLS_TextureFormat gpuformat)
	{
		KLS_FrameBuffer::addAttachmentDepth(name, gpuformat);
	}

	void KLS_OpenGL_FrameBuffer::getObjectIdFromColorBuffer(int x, int y, int index, KLS_UUID& id, int& meshbuffer)
	{
		KLS_Texture* t = getColorAttachment(index);
		t->bind(1);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, t->getCharData());
		glm::vec4 pixelData = t->getPixel(x, t->getHeight() - y);

		int objectId = 0;
		// Decode the object ID from the picked color
		objectId += int(pixelData.r * 256.0);
		objectId += int(pixelData.g * 65536.0);
		objectId += int(pixelData.b * 16777216.0);

		id = KLS_UUID(objectId);
		float meshbufferpixel = (float)pixelData.a * 255.0f;
		meshbuffer = static_cast<int>(meshbufferpixel);
	}

	void KLS_OpenGL_FrameBuffer::readColorBuffer(int index)
	{
		getColorAttachment(index)->bind(index);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, getColorAttachment(index)->getCharData());
	}

	glm::vec4 KLS_OpenGL_FrameBuffer::getPixelFromColorBuffer(int x, int y, int index)
	{
		readColorBuffer(index);
		return m_ColorAttachments[index]->getPixel(x, m_ColorAttachments[index]->getHeight() - y);
	}

	// force a resize of the framebuffer
	void KLS_OpenGL_FrameBuffer::resize(int width, int height)
	{
		if (m_Width != width || m_Height != height) 
		{
			cleanup();
			KLS_FrameBuffer::resize(width, height);
			create(getDriver(), m_Name, width, height, m_Resize, m_DrawBuffer);
			finalize();
		}
	}

	// recreate the framebuffer taking resize into account
	void KLS_OpenGL_FrameBuffer::recreate(int width, int height)
	{
		cleanup();
		if (m_Resize)
		{
			KLS_FrameBuffer::recreate(width, height);
			create(getDriver(), m_Name, width, height, m_Resize, m_DrawBuffer);
		}
		else
		{
			KLS_FrameBuffer::recreate(m_Width, m_Height);
			create(getDriver(), m_Name, m_Width, m_Height, m_Resize, m_DrawBuffer);
		}
		finalize();
	}

} // end namespace