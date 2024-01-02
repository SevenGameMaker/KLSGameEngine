// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

// unclutter the global namespace
namespace KLS
{
	// forward declarations
	typedef class KLS_Driver KLS_Driver;
	typedef class KLS_FrameBuffer KLS_FrameBuffer;
	typedef class KLS_FullscreenQuad KLS_FullscreenQuad;

	class KLS_API KLS_FrameBufferManager
	{
	private:
		KLS_VARIABLE_GET(KLS_Driver*, Driver, nullptr);
		KLS_VARIABLE_GET(KLS_FullscreenQuad*, FullScreenQuad, nullptr);
		std::vector<KLS_FrameBuffer*> m_FrameBuffers;
		KLS_VARIABLE_GET(std::string, CurrentFBO, "default");
		std::vector<KLS_MATERIAL_TYPE> m_PostProcessTechniques;

	public:
		KLS_FrameBufferManager();
		virtual ~KLS_FrameBufferManager();

		// set all variables to a knwon value
		virtual void initialize();

		// dual creation allows for better error handling
		virtual bool create(KLS_Driver* driver);

		// cleanup whatever memory mess we made
		virtual bool cleanup();

		virtual bool createFBOs();

		void returnToDefault(KLS_CLEARFLAG clearflags);

		void addFrameBuffer(KLS_FrameBuffer* fbo);

		virtual void beginFrame();
		virtual void postProcess(const double& elapsedtime);
		virtual void endFrame();

		int getPostProcessCount() { return (int)m_PostProcessTechniques.size(); }
		void postProcessAdd(KLS_MATERIAL_TYPE mt);
		bool postProcessExists(KLS_MATERIAL_TYPE mt);
		void postProcessRemove(KLS_MATERIAL_TYPE mt);
		void toggleEffect(KLS_MATERIAL_TYPE mt);

		KLS_FrameBuffer* getFrameBuffer(std::string name);
		void setCurrentFBO(std::string name, KLS_CLEARFLAG flags);

		virtual void onSystemFrameBufferResized();
	};


} // end namespace