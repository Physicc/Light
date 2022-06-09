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

#include "tools/Tracy.hpp"

#include "collider.hpp"

namespace Physicc
{
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
		ZoneScoped;

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
		m_transform = glm::translate(glm::mat4(1.0f), m_position);
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
		: Collider(position, rotation, scale)
	{
		ZoneScoped;
	}

	{
		using namespace BoxCollider;
		static_vertices = std::vector<glm::vec4>(8, glm::vec4(0, 0, 0, 1.0f)); //Static class member

		//Top-face vertices
		static_vertices[0] = glm::vec4(glm::vec3(0.5f), 1.0f);
		static_vertices[1] = static_vertices[0] - glm::vec4(1, 0, 0, 0);
		static_vertices[2] = static_vertices[0] - glm::vec4(0, 1, 0, 0);
		static_vertices[3] = static_vertices[0] - glm::vec4(1, 1, 0, 0);

		//Bottom-face vertices
		static_vertices[4] = glm::vec4(glm::vec3(-0.5f), 1.0f);
		static_vertices[5] = static_vertices[0] + glm::vec4(1, 0, 0, 0);
		static_vertices[6] = static_vertices[0] + glm::vec4(0, 1, 0, 0);
		static_vertices[7] = static_vertices[0] + glm::vec4(1, 1, 0, 0);
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
		ZoneScoped;

		glm::vec3 lowerBound(0.5f);
		glm::vec3 upperBound(-0.5f);

		std::vector<glm::vec4> standard_vertices = getVertices();

		for (int i = 0; i < 8; i++) //To be replaced by matrix multiplication
		{
			glm::vec3 temp = m_transform * standard_vertices[i]; 
			lowerBound = glm::min(lowerBound, temp); //Takes component-wise min
			upperBound = glm::max(upperBound, temp);
		}

		return {lowerBound, upperBound};
		//returning initializer list instead of an actual object
	}

	glm::vec3 BoxCollider::getCentroid() const
	{
		return m_position;
	}

	glm::vec3 BoxCollider::toBoxCoordinates(glm::vec3 point)
	{
		ZoneScoped;
		
		return glm::inverse(m_transform)*glm::vec4(point, 1);
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
		: Collider(position, rotation, scale), m_radius(radius)
	{
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
}

