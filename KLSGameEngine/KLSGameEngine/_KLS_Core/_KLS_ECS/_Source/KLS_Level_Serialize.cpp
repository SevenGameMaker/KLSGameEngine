// include the needed header files
#include "KLS_Level.h"
#include "KLS_Logger.h"
#include "KLS_Application.h"
#include "_KLS_Driver.h"
#include "KLS_Window.h"
#include "KLS_ImGui.h"
#include "KLS_Entity.h"
#include "KLS_ResourceManager.h"
#include "KLS_FrameBufferManager.h"
#include "KLS_FrameBuffer.h"
#include "KLS_UniformData.h"
#include "KLS_PhysX_World.h"
#include <fstream>
#include "KLS_ECS.h"
#include "KLS_Entity.h"
#include "_KLS_ECS_Components.h"

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

	template<>
	struct convert<KLS::KLS_UUID>
	{
		static Node encode(const KLS::KLS_UUID& rhs)
		{
			Node node;
			node.push_back(rhs);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, KLS::KLS_UUID& rhs)
		{
			if (!node.IsSequence())
				return false;

			rhs = node[0].as<UUID_TYPE>();
			return true;
		}
	};
}

// unclutter the global namespace
namespace KLS
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::quat& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	void KLS_Level::clear()
	{
		getECS()->clear();
	}

	void KLS_Level::serializeEntity(YAML::Emitter& out, KLS_Entity entity)
	{
		if (entity.hasComponent<KLS_COMPONENT_ID>())
		{
			auto& id = entity.getComponent<KLS_COMPONENT_ID>();
			out << YAML::Key << "Id" << YAML::Value << id.Id;
		}

		if (entity.hasComponent<KLS_COMPONENT_NAME>())
		{
			auto& tag = entity.getComponent<KLS_COMPONENT_NAME>();
			out << YAML::Key << "KLS_COMPONENT_NAME" << YAML::Value << tag.m_Name;
		}

		if (entity.hasComponent<KLS_COMPONENT_TRANSFORM>())
		{
			out << YAML::Key << "KLS_COMPONENT_TRANSFORM";
			out << YAML::BeginMap; // TransformComponent
			auto& tc = entity.getComponent<KLS_COMPONENT_TRANSFORM>();
			out << YAML::Key << "Translation" << YAML::Value << tc.m_Transform.m_Position;
			out << YAML::Key << "Rotation" << YAML::Value << tc.m_Transform.m_Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.m_Transform.m_Scale;
			out << YAML::EndMap; // TransformComponent
		}

		if (entity.hasComponent<KLS_COMPONENT_MESH>())
		{
			out << YAML::Key << "KLS_COMPONENT_MESH";
			out << YAML::BeginMap; // meshComponent
				auto& mesh = entity.getComponent<KLS_COMPONENT_MESH>();
				if (mesh.m_Mesh)
				{
					out << YAML::Key << "MeshFilename" << YAML::Value << mesh.m_Mesh->getName();
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
			out << YAML::EndMap; // meshComponent
		}
	}

	bool KLS_Level::saveToDisk(std::string filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		
		auto view = m_ECS->getView<KLS_COMPONENT_ID>();
		for (auto& e : view)
		{
			KLS_Entity entity(m_ECS, e);
			if (entity.isNull()) return true;

			// starting the map here allows us to override the serializeEntity function and capture application specific components
			// simply override the method, call this base class method first and then finish serializing in the overridden method
			out << YAML::BeginMap; // Entity
				serializeEntity(out, entity);
			out << YAML::EndMap; // Entity
		}
		
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath, std::ios::trunc);
		if (fout.is_open()) 
		{
			fout << out.c_str(); // Write the content of 'out' directly to the file
			fout.close(); // Don't forget to close the file when done
		}
		else 
		{
			KLS_ERROR("Unable to open / create save file {}", filepath.c_str());
			// Handle error: Unable to open the file
		}

		return true;
	}
	
	void KLS_Level::deserializeEntity(YAML::Node& data, KLS_Entity entity)
	{
		// check for meshes
		auto meshComponent = data["KLS_COMPONENT_MESH"];
		if (meshComponent)
		{
			KLS_ERROR("yep, mesh component");
		}
	}

	bool KLS_Level::loadFromDisk(std::string filepath)
	{
		YAML::Node data;
		data = YAML::LoadFile(filepath);
		KLS_CHECK((data.IsDefined()),"unable to open game save file",KLS_FAILURE);
		KLS_CHECK((data["Scene"]),"game save file is not the correct format",KLS_FAILURE);

		std::string sceneName = data["Scene"].as<std::string>();
		KLS_WARN("Deserializing scene {}", sceneName.c_str());

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				// Entities always have id, name, transform, cameradistance and children
				KLS_UUID uuid = entity["Id"].as<KLS_UUID>();

				// get the id and name to create the new entity
				std::string name;
				auto tagComponent = entity["KLS_COMPONENT_NAME"];
				if (tagComponent) name = tagComponent.as<std::string>();

				// create the new entity
				KLS_Entity deserializedEntity = m_ECS->createEntity(uuid, name,KLS_Transform());

				// deserialize the transform
				auto transformComponent = entity["KLS_COMPONENT_TRANSFORM"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.getComponent<KLS_COMPONENT_TRANSFORM>();
					tc.m_Transform.m_Position = transformComponent["Translation"].as<glm::vec3>();
					tc.m_Transform.m_Rotation = transformComponent["Rotation"].as<glm::quat>();
					tc.m_Transform.m_Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				// deserialize the camera distance
				auto cameraDistanceComponent = entity["KLS_COMPONENT_CAMERADISTANCE"];
				if (cameraDistanceComponent)
				{
					auto& cd = deserializedEntity.getComponent<KLS_COMPONENT_CAMERA_DISTANCE>();
					cd.m_CameraDistance = cameraDistanceComponent.as<float>();
				}

				deserializeEntity(entity, deserializedEntity);
			}
		}

		return true;
	}

} // end namespace