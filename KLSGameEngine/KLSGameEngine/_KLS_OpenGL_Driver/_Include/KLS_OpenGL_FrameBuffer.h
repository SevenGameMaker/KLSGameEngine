// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_FrameBuffer.h"
#include "KLS_UUID.h"

// unclutter the global namespace
namespace KLS
{
	class KLS_API KLS_OpenGL_FrameBuffer : public KLS_FrameBuffer
	{
	protected:

	public:
		// do nothing constructor / destructor (use initialize, create and cleanup instead)
		KLS_OpenGL_FrameBuffer();
		virtual ~KLS_OpenGL_FrameBuffer();

		// set all variables to a known value
		virtual void initialize();

		// dual creation allows for better error handling
		virtual bool create(KLS_Driver* driver, std::string name, int width, int height, bool resize, bool drawbuffer);

		// cleanup whatever memroy mess we made
		virtual bool cleanup();

		// bind / unbind the framebuffer
		virtual void bindForWrite();
		virtual void bindForRead(uint32_t slot);
		virtual void unbind();

		// final creation method after index and vertexbuffers are added
		virtual bool finalize();

		// add attachments
		virtual void addAttachmentColor(std::string name, KLS_TextureFormat gpuformat);
		virtual void addAttachmentDepth(KLS_Texture* previous);
		virtual void addAttachmentDepth(std::string name, KLS_TextureFormat gpuformat);

		// convert the fbo colorattachement pixel to an object id
		virtual void getObjectIdFromColorBuffer(int x, int y, int index, KLS_UUID& id, int& meshbuffer);

		// copy the colorattachement into the data() of the texture
		virtual void readColorBuffer(int index);

		// get the pixel of the fbo colorattachement
		virtual glm::vec4 getPixelFromColorBuffer(int x, int y, int index);

		// resize the framebuffer
		virtual void resize(int width, int height);

		// recreate the framebuffer
		virtual void recreate(int width, int height);
	};

} // end namespace
