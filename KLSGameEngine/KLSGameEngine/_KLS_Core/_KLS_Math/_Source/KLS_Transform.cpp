// include the needed header files
#include "KLS_Transform.h"

namespace KLS
{
    // Constructor to initialize the transform
    KLS_Transform::KLS_Transform() : m_ModelMatrix(1.0f), m_Position(0.0f), m_Rotation(1.0f, 0.0f, 0.0f, 0.0f), m_Scale(1.0f), m_Dirty(true) {}

    KLS_Transform::KLS_Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
        : m_ModelMatrix(1.0f), m_Position(position), m_Rotation(rotation), m_Scale(scale), m_Dirty(true) {}

    KLS_Transform::KLS_Transform(glm::vec3 position)
        : m_ModelMatrix(1.0f), m_Position(position), m_Rotation(KLS_IDENTITY_QUAT), m_Scale(glm::vec3(1)), m_Dirty(true) {}

    KLS_Transform::KLS_Transform(KLS_Transform& other)
    {
        if (this != &other)
        {
            m_ModelMatrix = other.m_ModelMatrix;
            m_Position = other.m_Position;
            m_Rotation = other.m_Rotation;
            m_Scale = other.m_Scale;
        }
        m_Dirty = true;
    }

    KLS_Transform::KLS_Transform(const KLS_Transform& other)
    {
        if (this != &other)
        {
            m_ModelMatrix = other.m_ModelMatrix;
            m_Position = other.m_Position;
            m_Rotation = other.m_Rotation;
            m_Scale = other.m_Scale;
        }
        m_Dirty = true;
    }

    // Assignment operator
    KLS_Transform& KLS_Transform::operator=(const KLS_Transform& other)
    {
        if (this != &other)
        {
            m_ModelMatrix = other.m_ModelMatrix;
            m_Position = other.m_Position;
            m_Rotation = other.m_Rotation;
            m_Scale = other.m_Scale;
            m_Dirty = true;
        }
        return *this;
    }

    // Addition operator
    KLS_Transform KLS_Transform::operator+(const KLS_Transform& other) const
    {
        KLS_Transform result(*this);
        result.m_Position += other.m_Position;
        result.m_Rotation *= other.m_Rotation;
        result.m_Scale *= other.m_Scale;
        result.m_Dirty = true; // Mark as dirty after addition
        return result;
    }

    // Compound addition operator
    KLS_Transform& KLS_Transform::operator+=(const KLS_Transform& other)
    {
        m_Position += other.m_Position;
        m_Rotation *= other.m_Rotation;
        m_Scale *= other.m_Scale;
        m_Dirty = true; // Mark as dirty after addition
        return *this;
    }

    // Equality operator
    bool KLS_Transform::operator==(const KLS_Transform& other) const
    {
        return (m_Position == other.m_Position &&
            m_Rotation == other.m_Rotation &&
            m_Scale == other.m_Scale);
    }

    // non const
    // Assignment operator
    KLS_Transform& KLS_Transform::operator=(KLS_Transform& other)
    {
        if (this != &other)
        {
            m_Position = other.m_Position;
            m_Rotation = other.m_Rotation;
            m_Scale = other.m_Scale;
            m_Dirty = true;
        }
        return *this;
    }

    // Addition operator
    KLS_Transform KLS_Transform::operator+(KLS_Transform& other) const
    {
        KLS_Transform result(*this);
        result.m_Position += other.m_Position;
        result.m_Rotation *= other.m_Rotation;
        result.m_Scale *= other.m_Scale;
        result.m_Dirty = true; // Mark as dirty after addition
        return result;
    }

    // Compound addition operator
    KLS_Transform& KLS_Transform::operator+=(KLS_Transform& other)
    {
        m_Position += other.m_Position;
        m_Rotation *= other.m_Rotation;
        m_Scale *= other.m_Scale;
        m_Dirty = true; // Mark as dirty after addition
        return *this;
    }

    // Equality operator
    bool KLS_Transform::operator==(KLS_Transform& other) const
    {
        return (m_Position == other.m_Position &&
            m_Rotation == other.m_Rotation &&
            m_Scale == other.m_Scale &&
            m_Dirty == other.m_Dirty);
    }

    // Function to update the transform's model matrix only when necessary
    glm::mat4& KLS_Transform::getModelMatrix()
    {
        if (m_Dirty)
        {
            // Recalculate the model matrix only if the transform has changed
            m_ModelMatrix = glm::mat4(1.0f);
            m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
            m_ModelMatrix *= glm::mat4_cast(m_Rotation);
            m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);

            // Reset the dirty flag
            m_Dirty = false;
        }

        return m_ModelMatrix;
    }

    // Setter functions to update transform properties
    void KLS_Transform::setPosition(const glm::vec3& v)
    {
        m_Position = v;
        m_Dirty = true;  // Mark the transform as dirty
    }

    void KLS_Transform::setRotation(const glm::quat& v)
    {
        m_Rotation = v;
        m_Dirty = true;  // Mark the transform as dirty
    }

    void KLS_Transform::setScale(const glm::vec3& v)
    {
        m_Scale = v;
        m_Dirty = true;  // Mark the transform as dirty
    }

    // Function to make the object look at a target position
    void KLS_Transform::lookAt(const glm::vec3& target)
    {
        glm::vec3 direction = glm::normalize(target - m_Position);
        glm::quat newRotation = glm::quatLookAt(glm::vec3(0.0f, 0.0f, 1.0f), direction);
        m_Rotation = newRotation;
        m_Dirty = true;  // Mark the transform as dirty
    }

    // Functions to get local coordinate axes based on rotation
    glm::vec3 KLS_Transform::getIn() const { return glm::normalize(glm::rotate(m_Rotation, glm::vec3(0.0f, 0.0f, -1.0f))); }
    glm::vec3 KLS_Transform::getLeft() const { return glm::normalize(glm::rotate(m_Rotation, glm::vec3(-1.0f, 0.0f, 0.0f))); }
    glm::vec3 KLS_Transform::getUp() const { return glm::normalize(glm::rotate(m_Rotation, glm::vec3(0.0f, 1.0f, 0.0f))); }

} // end namespace