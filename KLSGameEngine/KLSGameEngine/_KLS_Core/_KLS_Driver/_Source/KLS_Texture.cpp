// include th eneeded header files
#include "KLS_Texture.h"
#include "KLS_Logger.h"
#include "_KLS_Driver.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// unclutter the global namespace
namespace KLS
{
	int KLS_Texture::getBytesPerPixel(KLS_TextureFormat textureType)
	{
		switch (textureType)
		{
		case KLSTF_RG16F: return 4; // 2 channels * 2 bytes per channel
		case KLSTF_RGBA: return 4; // 4 channels * 1 byte per channel
		case KLSTF_RGB: return 3; // 3 channels * 1 byte per channel
		default:
			return 0;
		}
	}

	KLS_Texture::KLS_Texture()
	{
		// log this activity
		KLS_TRACE("  KLS_Texture::KLS_Texture()");
	}

	KLS_Texture::~KLS_Texture()
	{
		// log this activity
		KLS_TRACE("  KLS_Texture::~KLS_Texture({})", getName().c_str());

		if (m_Data) stbi_image_free(m_Data); m_Data = nullptr;
		m_Driver = nullptr;
	}

	bool KLS_Texture::create(KLS_Driver* driver, std::string name, int flip)
	{
		KLS_TRACE("  KLS_Texture::create({})", name.c_str());

		// rememebr this
		m_Driver = driver;
		m_Name = name;

		stbi_set_flip_vertically_on_load(flip);

		// attempt to load the file from disk
		m_Data = stbi_load(m_Name.c_str(), &m_Width, &m_Height, &m_Channels, 0);
		if (!m_Data)
		{
			KLS_WARN("failed to load texture from disk {}", name.c_str());
			return  KLS_FAILURE;
		}

		m_Format = KLSTF_RGBA;
		if (m_Channels == 3) m_Format = KLSTF_RGB;
		if (m_Channels == 4) m_Format = KLSTF_RGBA;

		// everything went fine
		return KLS_SUCCESS;
	}

	bool KLS_Texture::create(KLS_Driver* driver, std::string name, int flip,
		int width, int height, KLS_TextureFormat format)
	{
		// log this activity
		KLS_TRACE("  KLS_Texture::create({} {} {})", getName().c_str(), width, height);

		m_Driver = driver;
		m_Name = name;
		m_Width = width;
		m_Height = height;
		m_Format = format;

		// everything went fine
		return KLS_SUCCESS;
	}

	void KLS_Texture::setPixel(int x, int y, glm::vec4 color)
	{
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
		{
			// Out-of-bounds check
			return;
		}

		int bytesPerPixel = getBytesPerPixel(m_Format);

		switch (m_Format)
		{
		case KLSTF_RG16F: {
			unsigned char* dataPtr = static_cast<unsigned char*>(m_Data);
			unsigned char* pixelPtr = &dataPtr[(y * m_Width + x) * bytesPerPixel];
			//			unsigned char* pixelPtr = &m_Data[(y * m_Width + x) * bytesPerPixel];
			// Convert vec4 color to GL_RG data type (16-bit float)
			glm::vec2 rgColor = glm::vec2(color.r, color.g);
			float* pixelData = reinterpret_cast<float*>(pixelPtr);
			pixelData[0] = rgColor.r;
			pixelData[1] = rgColor.g;
			updateData();
			break;
		}
		case KLSTF_RGB: {
			unsigned char* dataPtr = static_cast<unsigned char*>(m_Data);
			unsigned char* pixelPtr = &dataPtr[(y * m_Width + x) * bytesPerPixel];
			//			unsigned char* pixelPtr = &m_Data[(y * m_Width + x) * bytesPerPixel];
			// Convert vec4 color to GL_RGB data type (8-bit unsigned byte)
			glm::u8vec3 rgbColor = glm::u8vec3(color.r * 255, color.g * 255, color.b * 255);
			pixelPtr[0] = rgbColor.r;
			pixelPtr[1] = rgbColor.g;
			pixelPtr[2] = rgbColor.b;
			updateData();
			break;
		}
		case KLSTF_RGBA: {
			unsigned char* dataPtr = static_cast<unsigned char*>(m_Data);
			unsigned char* pixelPtr = &dataPtr[(y * m_Width + x) * bytesPerPixel];
			//unsigned char* pixelPtr = &m_Data[(y * m_Width + x) * bytesPerPixel];
			// Convert vec4 color to GL_RGBA data type (8-bit unsigned byte)
			glm::u8vec4 rgbColor = glm::u8vec4(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
			pixelPtr[0] = rgbColor.r;
			pixelPtr[1] = rgbColor.g;
			pixelPtr[2] = rgbColor.b;
			pixelPtr[3] = rgbColor.a;
			updateData();
			break;
		}
		default:
			// Handle invalid texture type here
			break;
		}
	}

	glm::vec4 KLS_Texture::getPixel(int x, int y)
	{
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height) {
			// Out-of-bounds check
			KLS_ERROR("out of bounds in texture getpixel()");
			return glm::vec4(0.0f);
		}

		int bytesPerPixel = getBytesPerPixel(m_Format);

		switch (m_Format)
		{
		case KLSTF_RG16F:
		{
			unsigned char* dataPtr = static_cast<unsigned char*>(m_Data);
			unsigned char* pixelPtr = &dataPtr[(y * m_Width + x) * bytesPerPixel];
//			const unsigned char* pixelPtr = &m_Data[(y * m_Width + x) * bytesPerPixel];
			// Read GL_RG data type (16-bit float) and convert to vec4 color
			const float* pixelData = reinterpret_cast<const float*>(pixelPtr);
			glm::vec2 rgColor(pixelData[0], pixelData[1]);
			return glm::vec4(rgColor.r, rgColor.g, 0.0f, 1.0f);
			break;
		}
		case KLSTF_RGB:
		{
			unsigned char* dataPtr = static_cast<unsigned char*>(m_Data);
			unsigned char* pixelPtr = &dataPtr[(y * m_Width + x) * bytesPerPixel];
//			const unsigned char* pixelPtr = &m_Data[(y * m_Width + x) * bytesPerPixel];
			// Read GL_RGB data type (8-bit unsigned byte) and convert to vec4 color
			glm::u8vec3 rgbColor(pixelPtr[0], pixelPtr[1], pixelPtr[2]);
			return glm::vec4(rgbColor.r / 255.0f, rgbColor.g / 255.0f, rgbColor.b / 255.0f, 1.0f);
			break;
		}
		case KLSTF_RGBA:
		{
			unsigned char* dataPtr = static_cast<unsigned char*>(m_Data);
			unsigned char* pixelPtr = &dataPtr[(y * m_Width + x) * bytesPerPixel];
			//			const unsigned char* pixelPtr = &m_Data[(y * m_Width + x) * bytesPerPixel];
			// Read GL_RGB data type (8-bit unsigned byte) and convert to vec4 color
			glm::u8vec4 rgbColor(pixelPtr[0], pixelPtr[1], pixelPtr[2], pixelPtr[3]);
			return glm::vec4(rgbColor.r / 255.0f, rgbColor.g / 255.0f, rgbColor.b / 255.0f, rgbColor.a / 255.0f);
			break;
		}
		}

		KLS_ERROR("unkown format in texture getpixel()");
		return glm::vec4(0, 0, 0, 0);
	}

	void KLS_Texture::fill(glm::vec4 color)
	{
		int bytesPerPixel = getBytesPerPixel(m_Format);

		switch (m_Format)
		{
		case KLSTF_RG16F: 
		{
			// Convert vec4 color to GL_RG data type (16-bit float)
			glm::vec2 rgColor = glm::vec2(color.r, color.g);
			unsigned char* dataPtr = static_cast<unsigned char*>(m_Data);
			float* pixelData = reinterpret_cast<float*>(&dataPtr[0]);
//			float* pixelData = reinterpret_cast<float*>(&m_Data[0]);
			for (int y = 0; y < m_Height; ++y)
			{
				for (int x = 0; x < m_Width; ++x) 
				{
					pixelData[0] = rgColor.r;
					pixelData[1] = rgColor.g;
					pixelData += bytesPerPixel / sizeof(float);
				}
			}
			updateData();
			break;
		}
		case KLSTF_RGB: 
		{
			// Convert vec4 color to GL_RGB data type (8-bit unsigned byte)
			glm::u8vec3 rgbColor = glm::u8vec3(color.r * 255, color.g * 255, color.b * 255);
			unsigned char* dataPtr = static_cast<unsigned char*>(m_Data);
			unsigned char* pixelPtr = &dataPtr[0];
			//unsigned char* pixelPtr = &m_Data[0];
			for (int y = 0; y < m_Height; ++y)
			{
				for (int x = 0; x < m_Width; ++x) 
				{
					pixelPtr[0] = rgbColor.r;
					pixelPtr[1] = rgbColor.g;
					pixelPtr[2] = rgbColor.b;
					pixelPtr += bytesPerPixel;
				}
			}
			updateData();
			break;
		}
		case KLSTF_RGBA: 
		{
			// Convert vec4 color to GL_RGB data type (8-bit unsigned byte)
			glm::u8vec4 rgbColor = glm::u8vec4(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
			unsigned char* dataPtr = static_cast<unsigned char*>(m_Data);
			unsigned char* pixelPtr = &dataPtr[0];
//			unsigned char* pixelPtr = &m_Data[0];
			for (int y = 0; y < m_Height; ++y)
			{
				for (int x = 0; x < m_Width; ++x) 
				{
					pixelPtr[0] = rgbColor.r;
					pixelPtr[1] = rgbColor.g;
					pixelPtr[2] = rgbColor.b;
					pixelPtr[3] = rgbColor.a;
					pixelPtr += bytesPerPixel;
				}
			}
			updateData();
			break;
		}
					   // Add cases for other texture types as needed
		default:
			// Handle invalid texture type here
			break;
		}
	}

} // end namepsace