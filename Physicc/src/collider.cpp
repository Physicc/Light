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

namespace Physicc
{
	/**
	 * @brief Construct a new Collider::Collider object
	 * 
	 * @param position Position of the object. Default = (0,0,0)
	 * @param rotation Rotations about the axes. Default = (0,0,0)
	 * @param scale Length along each of the axes. Default = (1,1,1)
	 */
	Collider::Collider(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
		: m_position(position), m_rotate(rotation), m_scale(scale)
	{
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
	 * @param position Position of object in global space
	 * @param rotation Rotation about each of the axis in local space
	 * @param scale Scale of the object along each axis 
	 * 
	 */
	BoxCollider::BoxCollider(glm::vec3 position,
	                         glm::vec3 rotation,
	                         glm::vec3 scale)
		: Collider(position, rotation, scale),
		  m_vertices(std::vector<glm::vec4>(8, glm::vec4(0, 0, 0, 1.0f)))
	{
		//Top-face vertices
		m_vertices[0] = glm::vec4(scale * 0.5f, 0);
		m_vertices[1] = m_vertices[0] - glm::vec4(scale.x, 0, 0, 0);
		m_vertices[2] = m_vertices[0] - glm::vec4(0, scale.y, 0, 0);
		m_vertices[3] = m_vertices[0] - glm::vec4(scale.x, scale.y, 0, 0);

		//Bottom-face vertices
		m_vertices[4] = glm::vec4(scale * -0.5f, 0);
		m_vertices[5] = m_vertices[0] + glm::vec4(scale.x, 0, 0, 0);
		m_vertices[6] = m_vertices[0] + glm::vec4(0, scale.y, 0, 0);
		m_vertices[7] = m_vertices[0] + glm::vec4(scale.x, scale.y, 0, 0);
	}

	/**
	 * @brief Computes and returns Axis Aligned Bounding Box of Box shaped object
	 * 
	 * Computes location of vertices in global space and finds the 
	 * extreme points of AABB by comparing each component of every vertex
	 * 
	 * @return BoundingVolume::AABB
	 */
	BoundingVolume::AABB BoxCollider::getAABB() const
	{
		glm::vec3 lowerBound(0.5f);
		glm::vec3 upperBound(-0.5f);

		for (int i = 0; i < 8; i++)
		{
			glm::vec3 temp = m_transform * m_vertices[i];
			lowerBound = glm::min(lowerBound, temp); //Takes component-wise min
			upperBound = glm::max(upperBound, temp);
		}

		return {lowerBound, upperBound};
		//returning initializer list instead of an actual object
	}

	inline glm::vec3 BoxCollider::getCentroid() const
	{
		return m_position;
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
	 * @return BoundingVolume::AABB
	 */
	BoundingVolume::AABB SphereCollider::getAABB() const
	{
		glm::vec3 lowerBound = m_position - m_radius;
		glm::vec3 upperBound = m_position + m_radius;

		return {lowerBound, upperBound};
	}

	inline glm::vec3 SphereCollider::getCentroid() const
	{
		return m_position;
	}
}

