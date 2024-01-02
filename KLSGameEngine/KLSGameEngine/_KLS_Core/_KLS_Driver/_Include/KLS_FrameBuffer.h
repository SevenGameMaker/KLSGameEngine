// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_UUID.h"

// unclutter the global namespace
namespace KLS
{
	// forward declarations
	typedef class KLS_Driver KLS_Driver;
	typedef class KLS_Texture KLS_Texture;

	struct KLS_FrameBufferAttachmentInfo
	{
		KLS_FrameBufferAttachmentInfo(std::string name, KLS_TextureFormat format) :
			m_Name(name), m_GPUFormat(format) {}
		std::string m_Name;
		KLS_TextureFormat m_GPUFormat;

		bool m_Depth = false;
		bool m_DepthOwner = false;
		std::string m_DepthName;
	};

	class KLS_API KLS_FrameBuffer
	{
	protected:
		KLS_VARIABLE_GET(unsigned int, RenderID, 0);
		KLS_VARIABLE_GET(std::string, Name, KLS_DEFAULT_STRING);
		KLS_VARIABLE_GET(KLS_Driver*, Driver, nullptr);
		KLS_VARIABLE_GET(int, Width, 0);
		KLS_VARIABLE_GET(int, Height, 0);
		KLS_VARIABLE_GET(bool, Resize, true);
		KLS_VARIABLE_GET(bool, DrawBuffer, true);

		std::vector<KLS_Texture*> m_ColorAttachments;
		KLS_VARIABLE_GET(bool, DepthOwner, false);
		KLS_VARIABLE_GET(KLS_Texture*, DepthAttachment, nullptr);
	public:
		std::vector<KLS_FrameBufferAttachmentInfo> m_AttachmentInfo;

	public:
		// do nothing constructor / destructor (use initialize, create and cleanup instead)
		KLS_FrameBuffer();
		virtual ~KLS_FrameBuffer();

		// set all variables to a known value
		virtual void initialize();

		// dual creation allows for better error handling
		virtual bool create(KLS_Driver* driver, std::string name, int width, int height,bool resize, bool drawbuffer);

		// final creation method
		virtual bool finalize();

		// cleanup whatever memroy mess we made
		virtual bool cleanup();

		// bind methods
		virtual void bindForWrite();
		virtual void bindForRead(uint32_t slot);
		virtual void unbind() {};

		// Function to decode RGB channels into a float
		virtual void getObjectIdFromColorBuffer(int x, int y, int index, KLS_UUID& id, int& meshbuffer);
		virtual void readColorBuffer(int index);
		virtual glm::vec4 getPixelFromColorBuffer(int x, int y, int index);

		// get the attachments
		virtual size_t getColorAttachmentCount() { return m_ColorAttachments.size(); }
		virtual KLS_Texture* getColorAttachment(int index) { return m_ColorAttachments[index]; }

		// add attachements
		virtual void addAttachmentColor(std::string name, KLS_TextureFormat gpuformat);
		virtual void addAttachmentDepth(KLS_Texture* previous);
		virtual void addAttachmentDepth(std::string name, KLS_TextureFormat gpuformat);

		// get the aspect ration of this framebuffer
		virtual float getAspectRatio() { return (float)m_Width / (float)m_Height; }

		// force a resize of the framebuffer
		virtual void resize(int width, int height);

		// recreate the framebuffer taking resize into account
		void recreate(int width, int height);
	};

} // end namespace
