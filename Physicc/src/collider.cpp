#include "collider.hpp"

/**********************/
/*      Collider      */
/**********************/
Collider::Collider(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
        : m_position(position)
        , m_rotation(rotation)
        , m_scale(scale)
{
}


/*Getters*/
inline glm::vec3 Collider::getPos()
{
    return m_position;
}

inline glm::vec3 Collider::getRotate()
{
    return m_rotate;
}

inline glm::vec3 Collider::getScale()
{
    return m_scale;
}

inline glm::mat4 getTransform()
{
    return m_transform;
}

/*Setters*/
inline glm::vec3 Collider::setPos(glm::vec3 newpos)
{
    m_position = newpos;
}


inline glm::vec3 Collider::setPos(glm::vec3 newrotate)
{
    m_rotate = newrotate;
}


inline glm::vec3 Collider::setPos(glm::vec3 newscale)
{
    m_scale = newscale;
}

/*Update Transform*/
void Collider::updateTransform()
{
    m_transform = glm::translate(glm::mat4(1.0f), position);
	m_transform = glm::scale(m_transform, scale);
	m_transform = glm::rotate(m_transform, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
	m_transform = glm::rotate(m_transform, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
	m_transform = glm::rotate(m_transform, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
}


/**********************/
/*     BoxCollider    */
/**********************/

BoxCollider::BoxCollider(glm::vec3 size, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale )
                    )
                    : m_size(size), Collider(position,rotation,scale)
{
}

/**********************/
/*   SphereCollider   */
/**********************/
SphereCollider::SphereCollider(glm::vec3 size, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale )
                    )
                    : m_size(size), Collider(position,rotation,scale)
{
}
