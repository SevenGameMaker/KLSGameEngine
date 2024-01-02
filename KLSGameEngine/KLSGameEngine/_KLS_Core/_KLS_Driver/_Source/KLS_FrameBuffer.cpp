#include "KLS_FrameBuffer.h"
#include "_KLS_Driver.h"
#include "KLS_Logger.h"
#include "KLS_Texture.h"

namespace KLS
{
	// do nothing constructor / destructor (use initialize, create and cleanup instead)
	KLS_FrameBuffer::KLS_FrameBuffer()
	{
		// log this activity
		KLS_TRACE("KLS_FrameBuffer::KLS_FrameBuffer()");
	}

	KLS_FrameBuffer::~KLS_FrameBuffer()
	{
		// log this activity
		KLS_TRACE("KLS_FrameBuffer::~KLS_FrameBuffer()");
	}

	// set all variables to a known value
	void KLS_FrameBuffer::initialize()
	{
		// log this activity
		KLS_TRACE("KLS_FrameBuffer::initialize()");

		m_Driver = nullptr;
		m_ColorAttachments.clear();
		m_DepthAttachment = nullptr;
		m_DepthOwner = false;
	}

	// dual creation allows for better error handling
	bool KLS_FrameBuffer::create(KLS_Driver* driver, std::string name, int width, int height, bool resize, bool drawbuffer)
	{
		// log this activity
		KLS_TRACE("KLS_FrameBuffer::create()");

		m_Driver = driver;
		m_Name = name;
		m_Width = width;
		m_Height = height;
		m_Resize = resize;
		m_DrawBuffer = drawbuffer;

		// everything went fine
		return KLS_SUCCESS;
	}

	bool KLS_FrameBuffer::finalize()
	{
		// log this activity
		KLS_TRACE("KLS_FrameBuffer::finalize()");

		// everything went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memroy mess we made
	bool KLS_FrameBuffer::cleanup()
	{
		// log this activity
		KLS_TRACE("KLS_FrameBuffer::cleanup()");

		// always return false from a cleanup function
		return KLS_FAILURE;
	}

	void KLS_FrameBuffer::bindForWrite()
	{
	}

	void KLS_FrameBuffer::bindForRead(uint32_t slot)
	{
	}

	void KLS_FrameBuffer::addAttachmentColor(std::string name, KLS_TextureFormat gpuformat)
	{
		m_AttachmentInfo.push_back(KLS_FrameBufferAttachmentInfo(name, gpuformat));
	}

	void KLS_FrameBuffer::addAttachmentDepth(KLS_Texture* previous)
	{
		KLS_FrameBufferAttachmentInfo info(m_Name, previous->getFormat());
		info.m_Depth = true;
		info.m_DepthName = previous->getName();
		info.m_DepthOwner = false;
		m_AttachmentInfo.push_back(info);
	}

	void KLS_FrameBuffer::addAttachmentDepth(std::string name, KLS_TextureFormat gpuformat)
	{
		KLS_FrameBufferAttachmentInfo info(m_Name, gpuformat);
		info.m_Depth = true;
		info.m_DepthName = name;
		info.m_DepthOwner = true;
		m_AttachmentInfo.push_back(info);
	}

	void KLS_FrameBuffer::getObjectIdFromColorBuffer(int x, int y, int index, KLS_UUID& id, int& meshbuffer)
	{
		id = 0;
		meshbuffer = 0;
	}

	void KLS_FrameBuffer::readColorBuffer(int index)
	{
	}

	glm::vec4 KLS_FrameBuffer::getPixelFromColorBuffer(int x, int y, int index)
	{
		return glm::vec4(0);
	}
	
	void KLS_FrameBuffer::recreate(int width, int height)
	{
		m_Width = width;
		m_Height = height;
	}

	void KLS_FrameBuffer::resize(int width, int height)
	{
		m_Width = width;
		m_Height = height;
	}

} // end namespace