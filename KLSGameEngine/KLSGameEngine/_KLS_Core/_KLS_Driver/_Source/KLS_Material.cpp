#include "KLS_Material.h"
#include "KLS_Logger.h"
#include "KLS_String.h"

namespace KLS
{
	KLS_Material KLS_IDENTITY_MATERIAL;

	KLS_Material::KLS_Material()
	{
		// fill all of the arrays with some semi valid data
		fillWithString(Texture_Diffuse, KLS_MAX_TEXTURES_DIFFUSE, KLS_DEFAULT_TEXTURE_NAME);
		fillWithString(Texture_Specular, KLS_MAX_TEXTURES_SPECULAR, KLS_DEFAULT_TEXTURE_NAME);
		fillWithString(Texture_Normal, KLS_MAX_TEXTURES_NORMAL, KLS_DEFAULT_TEXTURE_NAME);
		fillWithString(Texture_Displacement, KLS_MAX_TEXTURES_DISPLACEMENT, KLS_DEFAULT_TEXTURE_NAME);
		fillWithString(Cubemap, KLS_MAX_CUBEMAPS, KLS_DEFAULT_TEXTURE_NAME);
		fillWithFloat(Repeat, KLS_MAX_REPEAT, 1.0f);
	}

	KLS_Material::~KLS_Material()
	{
	}

} // end namespace