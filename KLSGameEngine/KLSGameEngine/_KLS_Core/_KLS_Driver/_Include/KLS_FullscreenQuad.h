// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Material.h"

// unclutter the global namespace
namespace KLS
{
	typedef class KLS_Driver KLS_Driver;
	typedef class KLS_Mesh KLS_Mesh;

	class KLS_API KLS_FullscreenQuad
	{
	private:
		KLS_VARIABLE_GET(KLS_Driver*, Driver, nullptr);
		KLS_VARIABLE_GET(KLS_Mesh*, Quad, nullptr);

	public:
		KLS_FullscreenQuad(KLS_Driver* driver);
		virtual ~KLS_FullscreenQuad();

		virtual void setTexture(std::string name);
		virtual void setMaterialType(KLS_MATERIAL_TYPE type);

		virtual void draw();
	};

}// end namespace