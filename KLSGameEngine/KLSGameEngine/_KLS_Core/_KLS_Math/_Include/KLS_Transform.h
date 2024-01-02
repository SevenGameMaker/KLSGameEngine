// parse this file only once
#pragma once

// include the required header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

// unclutter the global namespace
namespace KLS
{
    class KLS_API KLS_Transform
    {
    public:
        glm::mat4 m_ModelMatrix;
        glm::vec3 m_Position;
        glm::quat m_Rotation = KLS_IDENTITY_QUAT;
        glm::vec3 m_Scale;
        bool m_Dirty = true;  // Flag to track if the transform has changed

        // Constructor to initialize the transform
        KLS_Transform();
        KLS_Transform(KLS_Transform& t);
        KLS_Transform(const KLS_Transform& t);
        KLS_Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale);
        KLS_Transform(glm::vec3 position);

        // Operator overloading
        KLS_Transform& operator=(const KLS_Transform& other);
        KLS_Transform operator+(const KLS_Transform& other) const;
        KLS_Transform& operator+=(const KLS_Transform& other);
        bool operator==(const KLS_Transform& other) const;

        KLS_Transform& operator=(KLS_Transform& other);
        KLS_Transform operator+(KLS_Transform& other) const;
        KLS_Transform& operator+=(KLS_Transform& other);
        bool operator==(KLS_Transform& other) const;

        // Function to update the transform's model matrix only when necessary
        glm::mat4& getModelMatrix();

        // Setter functions to update transform properties
        void setPosition(const glm::vec3& v);
        void setRotation(const glm::quat& v);
        void setScale(const glm::vec3& v);

        // Getter functions for transform properties
        const glm::vec3& getPosition() const { return m_Position; };
        const glm::quat& getRotation() const { return m_Rotation; };;
        const glm::vec3& getScale() const { return m_Scale; };;

        // Function to make the object look at a target position
        void lookAt(const glm::vec3& target);

        // Functions to get local coordinate axes based on rotation
        glm::vec3 getIn() const;
        glm::vec3 getLeft() const;
        glm::vec3 getUp() const;
    };

}  // end namespace
