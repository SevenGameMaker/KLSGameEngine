// indlcude the needed header files
#include "_KLS_Driver.h"
#include "KLS_Application.h"
#include "KLS_Timer.h"
#include "KLS_FrameBufferManager.h"
#include "KLS_Material.h"
#include "KLS_Pipeline.h"
#include "KLS_ResourceManager.h"
#include "KLS_Window.h"
#include "KLS_Camera.h"
#include "KLS_Buffer.h"
#include "KLS_UniformData.h"
#include "KLS_Level.h"

// unclutter the global namespace
namespace KLS
{
	void KLS_Driver::updateCamera(const double& elapsedtime)
	{
		KLS_Camera* cam = (m_ActiveCamera == nullptr ? m_DefaultCamera : m_ActiveCamera);
		cam->onAnimate(elapsedtime);
			setCameraPosition(cam->getTransform().getPosition());
			setViewMatrix(cam->getViewMatrix());
			setProjMatrix(cam->getProjMatrix(getWindow()->getRatio()));
			setOrthoMatrix(cam->getOrthoMatrix(getWindow()->getWidth(), getWindow()->getHeight()));
	}
	
	void KLS_Driver::updateUniforms()
	{
		// fill in a KLS_UniformData structure
		KLS_UniformData Data;
		Data.ProjMatrix = getProjMatrix();
		Data.ViewMatrix = getViewMatrix();
		Data.ClipPlane = getClipPlane();
		Data.CameraPos = glm::vec4(getCameraPosition(), 0);

		Data.AmbientColor = getAmbientColor();
		Data.Misc[0] = getAmbientIntensity();
		Data.Misc[1] = static_cast<float>(getNumberOfLights());
		Data.Misc[2] = (float)getTotalTime();

		// upload the KLS_UniformData structure 
		m_UniformBufferData->bind();
		m_UniformBufferData->prepare(&Data, sizeof(KLS_UniformData), 0);
	}

	void KLS_Driver::beginFrame()
	{
		// update the driver variables that the shaders might access
		setTotalTime(getApplication()->getTimer()->getTotalTime());

		// if we are using the framebuffers
		if (m_UseFBO)
		{
			// begin the framebuffer frame
			getFrameBufferManager()->beginFrame();
		}
		else
		{
			// otherwise just clear the glfw buffers
			clear(KLS_CLEARFLAG::KLSCF_ALL);
		}
	}

	void KLS_Driver::frame(const double &elapsedtime)
	{
		updateCamera(elapsedtime);
		updateUniforms();
	}

	void KLS_Driver::endFrame()
	{
		// if we are using the frame buffers
		if (m_UseFBO)
		{
			// if we are using the post processing then update them
			if (m_UsePostProcessing) getFrameBufferManager()->postProcess(getElapsedTime());

			// and end the framebuffer frame
			getFrameBufferManager()->endFrame();
		}
	}

	void KLS_Driver::swapBuffers()
	{
		// in the opengl driver we can just swap the buffers here
		getWindow()->swapBuffers();
	}

	void KLS_Driver::setClearColor(glm::vec4 color)
	{
		m_ClearColor = color;
	}

	void KLS_Driver::clear(KLS_CLEARFLAG flags)
	{
	}

	void KLS_Driver::setDepth(bool value)
	{
	}

	void KLS_Driver::setTransparency(bool value)
	{
	}

	void KLS_Driver::setViewport(int width, int height)
	{
	}

	void KLS_Driver::resetFBO(KLS_CLEARFLAG flags)
	{
	}

	// set the active material
	void KLS_Driver::setMaterial(const KLS_Material& material)
	{
		// if this is a different material then bind the new pipeline
		if (material.MaterialType != m_Material.MaterialType)
		{
			m_Pipeline = m_ResourceManager->getPipeline(material.MaterialType);
			if (m_Pipeline) m_Pipeline->bind();
		}

		// remember this
		m_Material = material;
	}
	void KLS_Driver::drawMesh(KLS_Mesh* mesh, glm::mat4& transform)
	{
	}

} // end namespace