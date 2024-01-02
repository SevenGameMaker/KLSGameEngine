#include "KLS_ResourceManager.h"
#include "KLS_Logger.h"
#include "KLS_Pipeline.h"
#include "_KLS_Driver.h"
#include "KLS_Pipeline.h"

// unclutter the global namespace
namespace KLS
{
	int KLS_ResourceManager::createPipeline(KLS_Driver* driver, std::string name,
		std::string vertexshaderfilename, std::string fragmentshaderfilename,
		KLS_ShaderCallback* callback)
	{
		KLS_Pipeline* pipeline = m_Driver->instantiatePipeline();
		pipeline->create(driver, name, vertexshaderfilename, fragmentshaderfilename, callback);
		return addPipeline(pipeline);
	}

	int KLS_ResourceManager::createPipelineWithGeom(KLS_Driver* driver, std::string name,
		std::string vertexshaderfilename, std::string geomshaderfilename, std::string fragmentshaderfilename,
		KLS_ShaderCallback* callback)
	{
		KLS_Pipeline* pipeline = m_Driver->instantiatePipeline();
		pipeline->create(driver, name, vertexshaderfilename, geomshaderfilename, fragmentshaderfilename, callback);
		return addPipeline(pipeline);
	}

	int KLS_ResourceManager::addPipeline(KLS_Pipeline* Pipeline)
	{
		if (!Pipeline)
		{
			KLS_WARN("attempting to add an invalid Pipeline");
			return 0;
		}

		// log this activity
		KLS_TRACE("KLS_ResourceManager::addPipeline({})", Pipeline->getName().c_str());

		// if the Pipeline is already in the map then let the user know and then bail
		for (uint32_t i = 0; i < m_Pipelines.size(); i++)
		{
			if (m_Pipelines[i] == Pipeline)
			{
				KLS_WARN("Pipeline {} already exists", Pipeline->getName().c_str());
				return Pipeline->getId();
			}
		}

		// add the Pipeline to the map
		m_Pipelines.push_back(Pipeline);

		m_PipelineCounter++;
		Pipeline->setId(m_PipelineCounter);

		// return where we added the pipeline
		return m_PipelineCounter;
	}

	void KLS_ResourceManager::removePipeline(int id)
	{
		// log this activity
		KLS_TRACE("KLS_ResourceManager::removePipeline({})", id);

		std::vector<KLS_Pipeline*>::iterator it;
		for (it = m_Pipelines.begin(); it != m_Pipelines.end(); it++)
		{
			if ((*it)->getId() == id)
			{
				delete (*it);
				m_Pipelines.erase(it);
				return;
			}
		}

		KLS_WARN("attempting to remove non existent Pipeline {}", id);
	}

	void KLS_ResourceManager::removeAllPipelines()
	{
		// log this activity
		KLS_TRACE("KLS_ResourceManager::removeAllPipelines()");

		// delete each entry and then clear the map
		std::vector<KLS_Pipeline*>::iterator it;
		for (it = m_Pipelines.begin(); it != m_Pipelines.end(); it++)
		{
			delete(*it);
		}

		m_Pipelines.clear();
	}

	KLS_Pipeline* KLS_ResourceManager::getPipeline(int id)
	{
		std::vector<KLS_Pipeline*>::iterator it;
		for (it = m_Pipelines.begin(); it != m_Pipelines.end(); it++)
		{
			if ((*it)->getId() == id) return (*it);
		}

		// something went terribly wrong here, we should at least have a default Pipeline to return?
		KLS_WARN("no Pipeline with id {} found", id);
		return nullptr;
	}

} // end namespace
