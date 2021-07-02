/** 
 * @file collider.cpp
 * @brief Contains the collider classes
 *
 * The Collider file contains the collider classes which hold the shape and
 * transform of the objects 
 *
 * @author Prakhar Mittal (prak74)
 * @author Tirthankar Mazumder (wermos)
 * @bug No known bugs.
 */

/* -- Includes -- */
/* collider header */

#include "collider.hpp"

/**
 * @brief Construct a new Collider:: Collider object
 * 
 * @param position Position of the object. Default = (0,0,0)
 * @param rotation Rotations about the axes. Default = (0,0,0)
 * @param scale Length along each of the axes. Default = (1,1,1)
 */
Collider::Collider(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: m_position(position), m_rotate(rotation), m_scale(scale)
{}

/**
 * @brief Get Position of object's center
 * 
 * @return glm::vec3 
 */
inline glm::vec3 Collider::getPos()
{
	return m_position;
}

/** 
 * @brief get Angle of rotation of the object about it's center
 * 
 * @return glm::vec3
 */
inline glm::vec3 Collider::getRotate()
{
	return m_rotate;
}

/** 
 * @brief get Scale of object
 * 
 * @return glm::vec3
 */
inline glm::vec3 Collider::getScale()
{
	return m_scale;
}

/** 
 * @brief get Transform matrix of object
 * 
 * @return glm::mat4
 */
inline glm::mat4 Collider::getTransform()
{
	return m_transform;
}

/** 
 * @brief set Position of object's center
 * 
 * @param newpos Takes the (x,y,z) coordinates to place the object at
 */
inline void Collider::setPos(glm::vec3 newpos)
{
	m_position = newpos;
}

/**
 * @brief Set rotation of object about it's center
 * 
 * @param newrotate vec3 containing rotation values about x,y,z axes
 */
inline void Collider::setRotate(glm::vec3 newrotate)
{
	m_rotate = newrotate;
}

/** 
 * @brief get Position of object's center
 * 
 * @param newscale New scale of the object
 */
inline void Collider::setScale(glm::vec3 newscale)
{
	m_scale = newscale;
}

/**
 * @brief Update Transform for rendering
 * 
 */
void Collider::updateTransform()
{
	m_transform = glm::translate(glm::mat4(1.0f), m_position);
	m_transform = glm::scale(m_transform, m_scale);
	m_transform = glm::rotate(m_transform,
	                          glm::radians(m_rotate.x),
	                          glm::vec3(1.0, 0.0, 0.0));
	m_transform = glm::rotate(m_transform,
	                          glm::radians(m_rotate.y),
	                          glm::vec3(0.0, 1.0, 0.0));
	m_transform = glm::rotate(m_transform,
	                          glm::radians(m_rotate.z),
	                          glm::vec3(0.0, 0.0, 1.0));
}

/**
 * @brief Creates a BoxCollider object
 * 
 * @param size Size of the object along each axes
 * @param position Position of object in global space
 * @param rotation Rotation about each of the axis in local space
 * @param scale Scale of the object along each axis 
 * 
 */
BoxCollider::BoxCollider(glm::vec3 size,
                         glm::vec3 position,
                         glm::vec3 rotation,
                         glm::vec3 scale)
	: m_size(size), Collider(position, rotation, scale)
{
	m_objectType = e_box;
}

/**
 * @brief Computes and returns Axis Aligned Bounding Box of Box shaped object
 * 
 * @return AABB 
 */
AABB BoxCollider::getAABB() const
{
	glm::vec3 lowerBound;
	glm::vec3 upperBound;
	lowerBound = m_position - m_size * 0.5f;
	upperBound = m_position + m_size * 0.5f;
	return AABB{lowerBound, upperBound};
}

/**
 * @brief Creates a SphereCollider object
 * 
 * @param radius Radius of the sphere
 * @param position Position of object in global space
 * @param rotation Rotation about each of the axis in local space
 * @param scale Scale of the object along each axis 
 * 
 */
SphereCollider::SphereCollider(float radius,
                               glm::vec3 position,
                               glm::vec3 rotation,
                               glm::vec3 scale)
	: m_radius(radius), Collider(position, rotation, scale)
{
	m_objectType = e_sphere;
}

/**
 * @brief Computes and returns Axis Aligned Bounding Box of Sphere shaped object
 * 
 * @return AABB 
 */
AABB SphereCollider::getAABB() const
{
	glm::vec3 lowerBound = m_position - m_radius;;
	glm::vec3 upperBound = m_position + m_radius;
	return AABB{lowerBound, upperBound};
}