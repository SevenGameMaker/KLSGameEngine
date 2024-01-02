#include "KLS_ResourceManager.h"
#include "KLS_Logger.h"
#include "_KLS_Driver.h"

#include "KLS_OpenGL_Pipeline.h"
#include "KLS_Camera.h"

namespace KLS
{
	class KLS_API KLS_OpenGL_ShaderCallback_FullscreenQuad : public KLS_ShaderCallback
	{
	public:
		virtual void setShaderParameters(KLS_Pipeline* pipeline, KLS_Material& material, glm::mat4x4 worldMatrix)
		{
			KLS_ShaderCallback::setShaderParameters(pipeline, material, worldMatrix);
			if (pipeline->getDriver()->getResourceManager()->bindTexture(1, material.Texture_Diffuse[0]))
				pipeline->setInt("Texture1", 1);
		}
	};

	class KLS_API KLS_OpenGL_ShaderCallback_Skybox : public KLS_ShaderCallback
	{
	public:
		virtual void setShaderParameters(KLS_Pipeline* pipeline, KLS_Material& material, glm::mat4x4 worldMatrix)
		{
			KLS_ShaderCallback::setShaderParameters(pipeline, material, worldMatrix);
			if (pipeline->getDriver()->getResourceManager()->bindCubemap(2, material.Cubemap[0]))
				pipeline->setInt("Texture2", 2);
		}
	};

	class KLS_OpenGL_ShaderCallback_Textured : public KLS_ShaderCallback
	{
	public:
		virtual void setShaderParameters(KLS_Pipeline* pipeline, KLS_Material& material, glm::mat4x4 worldMatrix)
		{
			KLS_ShaderCallback::setShaderParameters(pipeline, material, worldMatrix);
			if (pipeline->getDriver()->getResourceManager()->bindTexture(1, material.Texture_Diffuse[0]))
				pipeline->setInt("Texture1", 1);
		}
	};

	class KLS_OpenGL_ShaderCallback_Terrain : public KLS_ShaderCallback
	{
	public:
		virtual void setShaderParameters(KLS_Pipeline* pipeline, KLS_Material& material, glm::mat4x4 worldMatrix)
		{
			KLS_ShaderCallback::setShaderParameters(pipeline, material, worldMatrix);
			for (int x = 0; x < KLS_MAX_TEXTURES_DIFFUSE; x++)
			{
				std::string texture("Texture");
				texture += std::to_string(x + 1);
				if (pipeline->getDriver()->getResourceManager()->bindTexture(x + 1, material.Texture_Diffuse[x]))
					pipeline->setInt(texture, x + 1);
			}
		}
	};

	class KLS_OpenGL_ShaderCallback_AnimatedMesh : public KLS_ShaderCallback
	{
	public:
		virtual void setShaderParameters(KLS_Pipeline* pipeline, KLS_Material& material, glm::mat4x4 worldMatrix)
		{
			KLS_ShaderCallback::setShaderParameters(pipeline, material, worldMatrix);
			if (pipeline->getDriver()->getResourceManager()->bindTexture(1, material.Texture_Diffuse[0]))
				pipeline->setInt("Texture1", 1);

			for (int i = 0; i < material.FinalBoneMatrices.size(); ++i)
				pipeline->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", material.FinalBoneMatrices[i]);
		}
	};

	class KLS_OpenGL_ShaderCallback_Color : public KLS_ShaderCallback
	{
	public:
		virtual void setShaderParameters(KLS_Pipeline* pipeline, KLS_Material& material, glm::mat4x4 worldMatrix)
		{
			KLS_ShaderCallback::setShaderParameters(pipeline, material, worldMatrix);
			pipeline->setFloat4("uColor", material.ColorDiffuse);
		}
	};

	class KLS_OpenGL_ShaderCallback_Particles : public KLS_ShaderCallback
	{
	public:
		virtual void setShaderParameters(KLS_Pipeline* pipeline, KLS_Material& material, glm::mat4x4 worldMatrix)
		{
			KLS_ShaderCallback::setShaderParameters(pipeline, material, worldMatrix);
			if (pipeline->getDriver()->getResourceManager()->bindTexture(1, material.Texture_Diffuse[0]))
				pipeline->setInt("Texture1", 1);
		}
	};

	class KLS_OpenGL_ShaderCallback_Sun : public KLS_ShaderCallback
	{
	public:
		virtual void setShaderParameters(KLS_Pipeline* pipeline, KLS_Material& material, glm::mat4x4 worldMatrix)
		{
			KLS_ShaderCallback::setShaderParameters(pipeline, material, worldMatrix);
			if (pipeline->getDriver()->getResourceManager()->bindTexture(1, material.Texture_Diffuse[0]))
				pipeline->setInt("Texture1", 1);
			pipeline->setFloat4("uColor", material.ColorDiffuse);
		}
	};

	class KLS_OpenGL_ShaderCallback_Instanced : public KLS_ShaderCallback
	{
	public:
		virtual void setShaderParameters(KLS_Pipeline* pipeline, KLS_Material& material, glm::mat4x4 worldMatrix)
		{
			KLS_ShaderCallback::setShaderParameters(pipeline, material, worldMatrix);
			if (pipeline->getDriver()->getResourceManager()->bindTexture(1, material.Texture_Diffuse[0]))
				pipeline->setInt("Texture1", 1);
		}
	};

	class KLS_OpenGL_ShaderCallback_Bumpmap : public KLS_ShaderCallback
	{
	public:
		virtual void setShaderParameters(KLS_Pipeline* pipeline, KLS_Material& material, glm::mat4x4 worldMatrix)
		{
			KLS_ShaderCallback::setShaderParameters(pipeline, material, worldMatrix);
			if (pipeline->getDriver()->getResourceManager()->bindTexture(1, material.Texture_Diffuse[0]))
				pipeline->setInt("Texture1", 1);
			if (pipeline->getDriver()->getResourceManager()->bindTexture(2, material.Texture_Diffuse[1]))
				pipeline->setInt("Texture2", 2);
		}
	};

	class KLS_OpenGL_ShaderCallback_InfinitePlane : public KLS_ShaderCallback
	{
	public:
		virtual void setShaderParameters(KLS_Pipeline* pipeline, KLS_Material& material, glm::mat4x4 worldMatrix)
		{
			KLS_ShaderCallback::setShaderParameters(pipeline, material, worldMatrix);
			pipeline->setFloat("near", pipeline->getDriver()->getActiveCamera()->getNearPlane());
			pipeline->setFloat("far", pipeline->getDriver()->getActiveCamera()->getFarPlane());
		}
	};

	class KLS_API KLS_OpenGL_ShaderCallback_2DGreyScale : public KLS_ShaderCallback
	{
	public:
		virtual void setShaderParameters(KLS_Pipeline* pipeline, KLS_Material& material, glm::mat4x4 worldMatrix)
		{
			KLS_ShaderCallback::setShaderParameters(pipeline, material, worldMatrix);
			if (pipeline->getDriver()->getResourceManager()->bindTexture(1, material.Texture_Diffuse[0]))
				pipeline->setInt("Texture1", 1);
		}
	};

	class KLS_API KLS_OpenGL_ShaderCallback_2DInverted : public KLS_ShaderCallback
	{
	public:
		virtual void setShaderParameters(KLS_Pipeline* pipeline, KLS_Material& material, glm::mat4x4 worldMatrix)
		{
			KLS_ShaderCallback::setShaderParameters(pipeline, material, worldMatrix);
			if (pipeline->getDriver()->getResourceManager()->bindTexture(1, material.Texture_Diffuse[0]))
				pipeline->setInt("Texture1", 1);
		}
	};

}

namespace KLS
{
	bool KLS_ResourceManager::loadDefaults()
	{
		// these need to be in the same order as _KLS_Enums.h KLS_MATERIAL_TYPE{}
		switch (m_Driver->getDriverType())
		{
		case KLSDT_OPENGL:
		{
			createPipeline(m_Driver, "uv", SHADERPATH("uv.vert"), SHADERPATH("uv.frag"), new KLS_ShaderCallback_Default());
			createPipeline(m_Driver, "normals", SHADERPATH("normals.vert"), SHADERPATH("normals.frag"), new KLS_ShaderCallback_Default());
			createPipeline(m_Driver, "skybox", SHADERPATH("skybox.vert"), SHADERPATH("skybox.frag"), new KLS_OpenGL_ShaderCallback_Skybox());
			createPipeline(m_Driver, "textured", SHADERPATH("textured.vert"), SHADERPATH("textured.frag"), new KLS_OpenGL_ShaderCallback_Textured());
			createPipeline(m_Driver, "terrain", SHADERPATH("terrain.vert"), SHADERPATH("terrain.frag"), new KLS_OpenGL_ShaderCallback_Terrain());
			createPipeline(m_Driver, "fullscreenquad", SHADERPATHPE("fullscreenquad.vert"), SHADERPATHPE("fullscreenquad.frag"), new KLS_OpenGL_ShaderCallback_Terrain());
			createPipeline(m_Driver, "animatedmesh", SHADERPATH("animatedmesh.vert"), SHADERPATH("animatedmesh.frag"), new KLS_OpenGL_ShaderCallback_AnimatedMesh());
			createPipeline(m_Driver, "color", SHADERPATH("color.vert"), SHADERPATH("color.frag"), new KLS_OpenGL_ShaderCallback_Color());
			createPipelineWithGeom(m_Driver, "particles", SHADERPATH("particles.vert"), SHADERPATH("particles.gs"), SHADERPATH("particles.frag"), new KLS_OpenGL_ShaderCallback_Particles());
			createPipeline(m_Driver, "sun", SHADERPATH("sun.vert"), SHADERPATH("sun.frag"), new KLS_OpenGL_ShaderCallback_Sun());
			createPipeline(m_Driver, "instanced", SHADERPATH("instanced.vert"), SHADERPATH("textured.frag"), new KLS_OpenGL_ShaderCallback_Instanced());
			createPipeline(m_Driver, "bumpmap", SHADERPATH("bumpmap.vert"), SHADERPATH("bumpmap.frag"), new KLS_OpenGL_ShaderCallback_Bumpmap());
			createPipeline(m_Driver, "infiniteplane", SHADERPATH("infiniteplane.vert"), SHADERPATH("infiniteplane.frag"), new KLS_OpenGL_ShaderCallback_InfinitePlane());

			
			createPipeline(m_Driver, "2dgreyscale", SHADERPATHPE("2d_greyscale.vert"), SHADERPATHPE("2d_greyscale.frag"), new KLS_OpenGL_ShaderCallback_2DGreyScale());
			m_AvailablePostProcesssingMaterialTypes.push_back(KLSMT_2DGREYSCALE);
			createPipeline(m_Driver, "2dinverted", SHADERPATHPE("2d_inverted.vert"), SHADERPATHPE("2d_inverted.frag"), new KLS_OpenGL_ShaderCallback_2DInverted());
			m_AvailablePostProcesssingMaterialTypes.push_back(KLSMT_2DINVERTED);

		} break;
		}

		return KLS_SUCCESS;
	}

} // end namespace