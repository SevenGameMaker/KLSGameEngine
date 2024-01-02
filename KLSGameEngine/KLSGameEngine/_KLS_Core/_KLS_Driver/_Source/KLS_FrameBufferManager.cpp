// include the needed header files
#include "KLS_FrameBufferManager.h"
#include "KLS_Logger.h"
#include "_KLS_Driver.h"
#include "KLS_FrameBuffer.h"
#include "KLS_ResourceManager.h"
#include "KLS_Texture.h"
#include "KLS_FullscreenQuad.h"
#include "KLS_Window.h"

// unclutter the global namespace
namespace KLS
{
	KLS_FrameBufferManager::KLS_FrameBufferManager()
	{
		// log this activity
		KLS_TRACE("KLS_FrameBufferManager::KLS_FrameBufferManager()");
	}

	KLS_FrameBufferManager::~KLS_FrameBufferManager()
	{
		// log this activity
		KLS_TRACE("KLS_FrameBufferManager::~KLS_FrameBufferManager()");
	}

	// set all variables to a known value
	void KLS_FrameBufferManager::initialize()
	{
		// log this activity
		KLS_TRACE("KLS_FrameBufferManager::initialize()");

		m_Driver = nullptr;
		m_FullScreenQuad = nullptr;
		m_CurrentFBO = "";
		m_FrameBuffers.resize(0);
		m_FrameBuffers.clear();
	}

	// dual creation allows for better error handling
	bool KLS_FrameBufferManager::create(KLS_Driver* driver)
	{
		// log this activity
		KLS_TRACE("KLS_FrameBufferManager::create()");

		// remember this
		m_Driver = driver;

		m_FullScreenQuad = new KLS_FullscreenQuad(m_Driver);

		createFBOs();

		// everythign went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memory mess we made
	bool KLS_FrameBufferManager::cleanup()
	{
		// log this activity
		KLS_TRACE("KLS_FrameBufferManager::cleanup()");

		if (!m_FrameBuffers.empty())
		{
			for (int x = 0; x < m_FrameBuffers.size(); x++)
			{
				m_FrameBuffers[x]->cleanup();
				delete(m_FrameBuffers[x]);
			}
			m_FrameBuffers.clear();
		}

		if (m_FullScreenQuad) delete(m_FullScreenQuad); m_FullScreenQuad = nullptr;

		m_Driver = nullptr;

		// always return false from a cleanum function to allow for cascading errors
		return KLS_FAILURE;
	}

	void KLS_FrameBufferManager::addFrameBuffer(KLS_FrameBuffer* fbo)
	{
		m_FrameBuffers.push_back(fbo);
	}

	KLS_FrameBuffer* KLS_FrameBufferManager::getFrameBuffer(std::string name)
	{
		for (int x = 0; x < m_FrameBuffers.size(); x++)
		{
			if (m_FrameBuffers[x]->getName() == name) return m_FrameBuffers[x];
		}
		return nullptr;
	}

	bool KLS_FrameBufferManager::createFBOs()
	{
		KLS_INFO(" creating FBOs");

		if (!m_FrameBuffers.empty())
		{
			for (int x = 0; x < m_FrameBuffers.size(); x++)
			{
				m_FrameBuffers[x]->cleanup();
				delete(m_FrameBuffers[x]);
			}
			m_FrameBuffers.clear();
		}

		KLS_FrameBuffer* fbo = m_Driver->instantiateFrameBuffer();
			fbo->initialize();
			fbo->create(m_Driver, "fbo0", m_Driver->getWindow()->getWidth(), m_Driver->getWindow()->getHeight(),true, true);
			fbo->addAttachmentColor("fbo0_color", KLS_TextureFormat::KLSTF_RGBA);
			fbo->addAttachmentColor("fbo0_picker", KLS_TextureFormat::KLSTF_RGBA);
			fbo->addAttachmentDepth("fbo0_depth", KLSTF_DEPTH);
			fbo->finalize();
			addFrameBuffer(fbo);

			KLS_FrameBuffer* imguifbo = m_Driver->instantiateFrameBuffer();
				imguifbo->initialize();
				imguifbo->create(m_Driver, "imguifbo", m_Driver->getWindow()->getWidth(), m_Driver->getWindow()->getHeight(),false, true);
				imguifbo->addAttachmentColor("imguifbo_color", KLS_TextureFormat::KLSTF_RGBA);
				imguifbo->addAttachmentColor("imguifbo_picker", KLS_TextureFormat::KLSTF_RGBA);
				imguifbo->addAttachmentDepth("imguifbo_depth", KLSTF_DEPTH);
				imguifbo->finalize();
			addFrameBuffer(imguifbo);

		KLS_FrameBuffer* fbo2 = m_Driver->instantiateFrameBuffer();
			fbo2->initialize();
			fbo2->create(m_Driver, "fbo1", m_Driver->getWindow()->getWidth(), m_Driver->getWindow()->getHeight(),true, true);
			fbo2->addAttachmentColor("fbo1_color", KLS_TextureFormat::KLSTF_RGBA);
			fbo2->addAttachmentColor("fbo1_picker", KLS_TextureFormat::KLSTF_RGBA);
			fbo2->addAttachmentDepth(fbo->getDepthAttachment());
			fbo2->finalize();
			addFrameBuffer(fbo2);

		KLS_FrameBuffer* fbo3 = m_Driver->instantiateFrameBuffer();
			fbo3->initialize();
			fbo3->create(m_Driver, "reflective", m_Driver->getWindow()->getWidth(), m_Driver->getWindow()->getHeight(),true, true);
			fbo3->addAttachmentColor("reflective_color", KLS_TextureFormat::KLSTF_RGBA);
			fbo3->addAttachmentDepth(fbo->getDepthAttachment());
			fbo3->finalize();
			addFrameBuffer(fbo3);

		KLS_FrameBuffer* fbo4 = m_Driver->instantiateFrameBuffer();
			fbo4->initialize();
			fbo4->create(m_Driver, "refractive", m_Driver->getWindow()->getWidth(), m_Driver->getWindow()->getHeight(),true, true);
			fbo4->addAttachmentColor("refractive_color", KLS_TextureFormat::KLSTF_RGBA);
			fbo4->addAttachmentDepth(fbo->getDepthAttachment());
			fbo4->finalize();
			addFrameBuffer(fbo4);

		return KLS_SUCCESS;
	}

	void KLS_FrameBufferManager::setCurrentFBO(std::string name, KLS_CLEARFLAG clearflags)
	{
		KLS_FrameBuffer* fbo = getFrameBuffer(name);
		if (fbo)
		{
			fbo->bindForWrite();
			if (clearflags > 0) getDriver()->clear(clearflags);
			getDriver()->setViewport(fbo->getWidth(), fbo->getHeight());
			m_CurrentFBO = name;
		}
		else
		{
			getDriver()->resetFBO(clearflags);
			m_CurrentFBO = "default";
		}
	}

	void KLS_FrameBufferManager::beginFrame()
	{
		setCurrentFBO("fbo0", KLSCF_ALL);
	}

	void KLS_FrameBufferManager::returnToDefault(KLS_CLEARFLAG clearflags)
	{
		getDriver()->resetFBO(clearflags);
	}

	void KLS_FrameBufferManager::postProcess(const double& elapsedtime)
	{
		if (!m_PostProcessTechniques.empty())
		{
			for (int x = 0; x < m_PostProcessTechniques.size(); x++)
			{
				if (m_CurrentFBO == "fbo0")
				{
					getFrameBuffer(m_CurrentFBO)->bindForRead(1);

					m_FullScreenQuad->setTexture(getFrameBuffer(m_CurrentFBO)->getColorAttachment(0)->getName());
					m_FullScreenQuad->setMaterialType(m_PostProcessTechniques[x]);

					setCurrentFBO("fbo1", KLSCF_ALL);
					m_FullScreenQuad->draw();
				}
				else
				{
					getFrameBuffer(m_CurrentFBO)->bindForRead(1);

					m_FullScreenQuad->setTexture(getFrameBuffer(m_CurrentFBO)->getColorAttachment(0)->getName());
					m_FullScreenQuad->setMaterialType(m_PostProcessTechniques[x]);

					setCurrentFBO("fbo0", KLSCF_ALL);
					m_FullScreenQuad->draw();
				}
			}
		}
	}

	void KLS_FrameBufferManager::endFrame()
	{
		returnToDefault(KLSCF_ALL);
		getFrameBuffer(m_CurrentFBO)->bindForRead(1);
		getDriver()->setDepth(false);
		m_FullScreenQuad->setTexture(getFrameBuffer(m_CurrentFBO)->getColorAttachment(0)->getName());
		m_FullScreenQuad->setMaterialType(KLSMT_FSQUAD);
		m_FullScreenQuad->draw();
	}

	void KLS_FrameBufferManager::postProcessAdd(KLS_MATERIAL_TYPE mt)
	{
		m_PostProcessTechniques.push_back(mt);
	}

	bool KLS_FrameBufferManager::postProcessExists(KLS_MATERIAL_TYPE mt)
	{
		if (m_PostProcessTechniques.empty()) return KLS_FAILURE;
		for (int x = 0; x < m_PostProcessTechniques.size(); x++)
		{
			if (m_PostProcessTechniques[x] == mt) return KLS_SUCCESS;
		}
		return KLS_FAILURE;
	}

	void KLS_FrameBufferManager::postProcessRemove(KLS_MATERIAL_TYPE mt)
	{
		if (m_PostProcessTechniques.empty()) return;
		m_PostProcessTechniques.erase(std::remove_if(m_PostProcessTechniques.begin(), m_PostProcessTechniques.end(), [&](const KLS_MATERIAL_TYPE& item) {
			return item == mt;
			}), m_PostProcessTechniques.end());
	}

	void KLS_FrameBufferManager::toggleEffect(KLS_MATERIAL_TYPE mt)
	{
		if (postProcessExists(mt)) postProcessRemove(mt);
		else postProcessAdd(mt);
	}

	void KLS_FrameBufferManager::onSystemFrameBufferResized()
	{
		KLS_INFO(" KLS_FrameBufferManager::onSystemFrameBufferResized()");

		int width = getDriver()->getWindow()->getWidth();
		int height = getDriver()->getWindow()->getHeight();
		if (!m_FrameBuffers.empty())
		{
			for (int x = 0; x < m_FrameBuffers.size(); x++)
			{
				m_FrameBuffers[x]->recreate(width,height);
			}
		}
	}

} // end namespace