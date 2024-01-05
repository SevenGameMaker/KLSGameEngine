#pragma once

// include the needed header files
#include "KLS_Logger.h"
#include <fstream>
#include "KLS_ECS.h"
#include "KLS_Entity.h"
#include "yaml-cpp/yaml.h"


namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::quat>
	{
		static Node encode(const glm::quat& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::quat& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

// unclutter the global namespace
namespace KLS
{
	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::quat& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	inline std::string getAsString(YAML::Node& data, std::string title)
	{
		auto text = data[title.c_str()];
		if (text) return text.as<std::string>();
		return KLS_DEFAULT_STRING;
	}
	inline bool getAsBool(YAML::Node& data, std::string title)
	{
		auto text = data[title.c_str()];
		if (text) return text.as<bool>();
		return false;
	}
	inline int getAsInt(YAML::Node& data, std::string title)
	{
		auto text = data[title.c_str()];
		if (text) return text.as<int>();
		return 0;
	}
	inline 	float getAsFloat(YAML::Node& data, std::string title)
	{
		auto text = data[title.c_str()];
		if (text) return text.as<float>();
		return 0.0f;
	}
	inline 	glm::vec4 getAsVec4(YAML::Node& data, std::string title)
	{
		auto text = data[title.c_str()];
		if (text) return text.as<glm::vec4>();
		return glm::vec4(0);
	}


} // end namepsace


/*
		if (entity.hasComponent<KLS_COMPONENT_MESH>())
		{
			out << YAML::Key << "KLS_COMPONENT_MESH";
			out << YAML::BeginMap; // meshComponent
				auto& mesh = entity.getComponent<KLS_COMPONENT_MESH>();
				if (mesh.m_Mesh)
				{
					out << YAML::Key << "MeshFilename" << YAML::Value << mesh.m_Mesh->getName();
					out << YAML::Key << "VertexType" << YAML::Value << (int)mesh.m_Mesh->getVertexType();
					out << YAML::Key << "MaterialCount" << YAML::Value << mesh.m_Mesh->getMaterialCount();

					for (int x = 0; x < mesh.m_Mesh->getMaterialCount(); x++)
					{
						std::string mattext = std::string("Material") + std::to_string(x);
						out << YAML::Key << mattext;
						out << YAML::BeginMap; // current material
						out << YAML::Key << "Primitive" << YAML::Value << (int)mesh.m_Mesh->getMaterial(x).Primitive;
						out << YAML::Key << "MaterialType" << YAML::Value << (int)mesh.m_Mesh->getMaterial(x).MaterialType;
						out << YAML::Key << "ColorDiffuse" << YAML::Value << mesh.m_Mesh->getMaterial(x).ColorDiffuse;
						out << YAML::Key << "ColorAmbient" << YAML::Value << mesh.m_Mesh->getMaterial(x).ColorAmbient;
						out << YAML::Key << "ColorSpecular" << YAML::Value << mesh.m_Mesh->getMaterial(x).ColorSpecular;
						out << YAML::Key << "ColorEmmissive" << YAML::Value << mesh.m_Mesh->getMaterial(x).ColorEmmissive;
						out << YAML::Key << "Shininess" << YAML::Value << mesh.m_Mesh->getMaterial(x).Shininess;
						out << YAML::Key << "Transparency" << YAML::Value << mesh.m_Mesh->getMaterial(x).Transparency;
						out << YAML::Key << "Wireframe" << YAML::Value << mesh.m_Mesh->getMaterial(x).Wireframe;
						out << YAML::Key << "InstanceCount" << YAML::Value << mesh.m_Mesh->getMaterial(x).InstanceCount;
						out << YAML::Key << "Mix" << YAML::Value << mesh.m_Mesh->getMaterial(x).Mix;
						out << YAML::Key << "ClipPlane" << YAML::Value << mesh.m_Mesh->getMaterial(x).ClipPlane;

						out << YAML::Key << "Repeats" << YAML::Value << mesh.m_Mesh->getMaterial(x).Repeat.size();
						for (int m = 0; m < mesh.m_Mesh->getMaterial(x).Repeat.size(); m++)
						{
							std::string repeattext = std::string("Repeat") + std::to_string(m);
							out << YAML::Key << repeattext << YAML::Value << mesh.m_Mesh->getMaterial(x).Repeat[m];
						}

						out << YAML::Key << "TextureDiffuseCount" << YAML::Value << mesh.m_Mesh->getMaterial(x).Texture_Diffuse.size();
						for (int td = 0; td < mesh.m_Mesh->getMaterial(x).Texture_Diffuse.size(); td++)
						{
							std::string tdtext = std::string("TextureDiffuse") + std::to_string(td);
							out << YAML::Key << tdtext << YAML::Value << mesh.m_Mesh->getMaterial(x).Texture_Diffuse[td];
						}

						out << YAML::EndMap; // current material
					}
			}
*/