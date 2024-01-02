// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include <imgui.h>

// unclutter the global namespace
namespace KLS
{
	// forward declarations
	typedef class KLS_Application KLS_Application;
	typedef class KLS_Driver KLS_Driver;
	typedef class KLS_Texture KLS_Texture;
	typedef class KLS_Texture_Cubemap KLS_Texture_Cubemap;
	typedef class KLS_Pipeline KLS_Pipeline;
	typedef class KLS_Mesh KLS_Mesh;
	typedef class KLS_AnimatedMesh KLS_AnimatedMesh;
	typedef class KLS_ShaderCallback KLS_ShaderCallback;

	/*
	*
	*/
	class KLS_API KLS_ResourceManager
	{
	private:
		KLS_VARIABLE_GET(KLS_Application*, Application, nullptr);
		KLS_VARIABLE_GET(KLS_Driver*, Driver, nullptr);

		std::multimap<std::string, KLS_Mesh*> m_Meshes;				// list of available meshes
		std::multimap<std::string, KLS_Texture*> m_Textures;			// list of available textures
		std::vector<std::string> m_FailedTextures;							// list of textures that failed to load
		std::multimap<std::string, KLS_Texture_Cubemap*> m_Cubemaps;	// list of available cubemaps
		std::vector<KLS_Pipeline*> m_Pipelines;								// list of available pipelines		


		std::vector<KLS_MATERIAL_TYPE> m_AvailablePostProcesssingMaterialTypes; // list of available postprocessing types		

		KLS_VARIABLE_SETGET(int, PipelineCounter, 0);						// simple variable to provide unique id's

	public:
		// class constructor / destructor (use create / cleanup instead)
		KLS_ResourceManager();
		virtual ~KLS_ResourceManager();

		// dual creation allows for better error handling
		virtual bool create(KLS_Application* app);

		// cleanup whatever memory mess we made
		virtual bool cleanup();

		// load up the default items
		virtual bool loadDefaults();

		// list of available postprocessing material types
		std::vector<KLS_MATERIAL_TYPE>& getAvailablePostProcesssingMaterialTypes() { return m_AvailablePostProcesssingMaterialTypes; }

		// instance creation methods
		virtual KLS_Texture* createTexture(std::string filename, int flip);
		virtual KLS_Texture* createTexture(std::string name, int flip, int width, int height, KLS_TextureFormat format);
		virtual KLS_Texture_Cubemap* createCubemap(std::string name, int flip, std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back);

		// texture storage related methods
		KLS_Texture* getTexture(std::string name, int flip);
		void addTexture(KLS_Texture* texture);
		void removeTexture(std::string name);
		void removeAllTextures();
		void renameTexture(std::string old, std::string newname);
		bool isInFailedTextureList(const std::string& name) const;
		bool bindTexture(int slot, std::string name);
		ImTextureID getTextureId(std::string name); 	// useful for imgui rendering of textures
		void logTextures();
		void logFailedTextures();

		// cubemap related methods
		KLS_Texture_Cubemap* getCubemap(std::string name);
		void addCubemap(KLS_Texture_Cubemap* texture);
		void removeCubemap(std::string name);
		void removeAllCubemaps();
		bool bindCubemap(int slot, std::string name);

		// Pipeline related methods
		KLS_Pipeline* getPipeline(int id);
		int createPipeline(KLS_Driver* driver, std::string name,
			std::string vertexshaderfilename, std::string fragmentshaderfilename,
			KLS_ShaderCallback* callback);
		int createPipelineWithGeom(KLS_Driver* driver, std::string name,
			std::string vertexshaderfilename, std::string geomshaderilename, std::string fragmentshaderfilename,
			KLS_ShaderCallback* callback);
		int addPipeline(KLS_Pipeline* Pipeline);
		void removePipeline(int id);
		void removeAllPipelines();

		// model methods
		KLS_AnimatedMesh* m_AnimationsTempMesh = nullptr;
		KLS_Mesh* getMesh(std::string name, KLS_VERTEX_TYPE type = KLSVT3D);
		void removeAllMeshes();
		KLS_AnimatedMesh* createAnimatedMesh(KLS_VERTEX_TYPE type, std::string name);
		void removeMesh(KLS_Mesh* mesh);
		KLS_Mesh* createMesh(KLS_VERTEX_TYPE type, std::string name, std::vector<float> vertices, std::vector<uint32_t> indices);
	private:
		KLS_Mesh* createMesh(KLS_VERTEX_TYPE type, std::string name);
	};


} // end namespace