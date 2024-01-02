// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Colors.h"
#include "KLS_UUID.h"

// unclutter the global namespace
namespace KLS
{
	// structure used in the rendering phase to send pertinent data to the shader uniforms
	class KLS_API KLS_Material
	{
	public:
		// determines rendering pipeline
		KLS_UUID			ObjectId = 0;
		int					MeshBuffer = 0;
		bool				Hilite = 0;
		KLS_PRIMITIVE_TYPE	Primitive = KLS_PT_TRIANGLE;
		int					MaterialType = 0;
		glm::vec4			ColorDiffuse = KLS_WHITE;		// material diffuse color
		glm::vec4			ColorAmbient = KLS_WHITE;		// material ambient color
		glm::vec4			ColorSpecular = KLS_BLACK;		// material specular color
		glm::vec4			ColorEmmissive = KLS_BLACK;		// material emmissive color
		float				Shininess = 0.0f;				// shininess (1 = metal)
		bool				Transparency = false;			// transparency flag
		bool				Wireframe = false;			// determines if shader renders in wireframe mode
		int					InstanceCount = 0;

		std::vector<std::string> Texture_Diffuse;				// list of texture filenames
		std::vector<std::string> Texture_Specular;				// list of texture filenames
		std::vector<std::string> Texture_Normal;				// list of texture filenames
		std::vector<std::string> Texture_Displacement;			// list of texture filenames
		std::vector<std::string> Cubemap;						// list of cubemap names

		float				Mix = 0.5;				// shader constant
		std::vector<float>	Repeat;								// list of texture repeat values
		glm::vec4 ClipPlane = glm::vec4(0, 1, 0, 10000);		// shader clippolane value

		std::vector<glm::mat4> FinalBoneMatrices;
	public:
		KLS_Material();
		~KLS_Material();

		// Copy Constructor
		KLS_Material(const KLS_Material& other)
		{
			// Copy each member variable from the 'other' object to this object
			this->ObjectId = other.ObjectId;
			this->Primitive = other.Primitive;
			this->MaterialType = other.MaterialType;
			this->ColorDiffuse = other.ColorDiffuse;
			this->ColorAmbient = other.ColorAmbient;
			this->ColorSpecular = other.ColorSpecular;
			this->ColorEmmissive = other.ColorEmmissive;
			this->Shininess = other.Shininess;
			this->Transparency = other.Transparency;
			this->Wireframe = other.Wireframe;
			this->Texture_Diffuse = other.Texture_Diffuse;
			this->Texture_Specular = other.Texture_Specular;
			this->Texture_Normal = other.Texture_Normal;
			this->Texture_Displacement = other.Texture_Displacement;
			this->Cubemap = other.Cubemap;
			this->Mix = other.Mix;
			this->Repeat = other.Repeat;
			this->ClipPlane = other.ClipPlane;
			this->FinalBoneMatrices = other.FinalBoneMatrices;
			this->InstanceCount = other.InstanceCount;
			this->MeshBuffer = other.MeshBuffer;
			this->Hilite = other.Hilite;
		}

		// Assignment Operator Override
		KLS_Material& operator=(const KLS_Material& other)
		{
			if (this == &other)
			{
				// Self-assignment check to avoid unnecessary work
				return *this;
			}

			// Copy each member variable from the 'other' object to this object
			this->ObjectId = other.ObjectId;
			this->Primitive = other.Primitive;
			this->MaterialType = other.MaterialType;
			this->ColorDiffuse = other.ColorDiffuse;
			this->ColorAmbient = other.ColorAmbient;
			this->ColorSpecular = other.ColorSpecular;
			this->ColorEmmissive = other.ColorEmmissive;
			this->Shininess = other.Shininess;
			this->Transparency = other.Transparency;
			this->Wireframe = other.Wireframe;
			this->Texture_Diffuse = other.Texture_Diffuse;
			this->Texture_Specular = other.Texture_Specular;
			this->Texture_Normal = other.Texture_Normal;
			this->Texture_Displacement = other.Texture_Displacement;
			this->Cubemap = other.Cubemap;
			this->Mix = other.Mix;
			this->Repeat = other.Repeat;
			this->ClipPlane = other.ClipPlane;
			this->FinalBoneMatrices = other.FinalBoneMatrices;
			this->InstanceCount = other.InstanceCount;
			this->MeshBuffer = other.MeshBuffer;
			this->Hilite = other.Hilite;

			return *this; // Return a reference to this object after assignment
		}

		// Assignment Operator Override
		KLS_Material operator=(KLS_Material& other)
		{
			if (this == &other)
			{
				// Self-assignment check to avoid unnecessary work
				return *this;
			}

			// Copy each member variable from the 'other' object to this object
			this->ObjectId = other.ObjectId;
			this->Primitive = other.Primitive;
			this->MaterialType = other.MaterialType;
			this->ColorDiffuse = other.ColorDiffuse;
			this->ColorAmbient = other.ColorAmbient;
			this->ColorSpecular = other.ColorSpecular;
			this->ColorEmmissive = other.ColorEmmissive;
			this->Shininess = other.Shininess;
			this->Transparency = other.Transparency;
			this->Wireframe = other.Wireframe;
			this->Texture_Diffuse = other.Texture_Diffuse;
			this->Texture_Specular = other.Texture_Specular;
			this->Texture_Normal = other.Texture_Normal;
			this->Texture_Displacement = other.Texture_Displacement;
			this->Cubemap = other.Cubemap;
			this->Mix = other.Mix;
			this->Repeat = other.Repeat;
			this->ClipPlane = other.ClipPlane;
			this->FinalBoneMatrices = other.FinalBoneMatrices;
			this->InstanceCount = other.InstanceCount;
			this->MeshBuffer = other.MeshBuffer;
			this->Hilite = other.Hilite;

			return *this; // Return a reference to this object after assignment
		}

		// clear the texture lists. 
		void clear()
		{
			Texture_Diffuse.clear();
			Texture_Specular.clear();
			Texture_Normal.clear();
			Texture_Displacement.clear();
			Cubemap.clear();
			Repeat.clear();
		}
	};

	// the default material
	extern KLS_API KLS_Material KLS_IDENTITY_MATERIAL;

} // end namespace