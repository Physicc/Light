#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "light.hpp"

class Collider
{
    public:
        Collider(glm::vec3 position = {0}, glm::vec3 rotation = {0}, glm::vec3 scale = {0})
        virtual ~Collider() = 0;        //Make pure virtual class so no-instantiation possible

        inline glm::vec3 getPos();
        inline glm::vec3 getRotate();
        inline glm::vec3 getScale();
        inline glm::mat4 getTransform();

        inline glm::vec3 setPos(glm::vec3 newpos);
        inline glm::vec3 setRotate(glm::vec3 newrotate);
        inline glm::vec3 setScale(glm::vec3 newscale);

        void updateTransform();
    
    private:
        glm::vec3 m_positon;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;
        glm::mat4 m_transform;

}

class BoxCollider : public Collider
{
    public:
        BoxCollider(glm::vec3 size = {1}, glm::vec3 position = {0}, glm::vec3 rotation = {0}, glm::vec3 scale = {0});

        ~BoxCollider() = default;

    private:
        glm::vec3 m_size;
}

class SphereCollider : public Collider
{
    public:
        SphereCollider(glm::scalar radius = {1}, glm::vec3 position = {0}, glm::vec3 rotation = {0}, glm::vec3 scale = {0});

        ~SphereCollider() = default;

    private:
        glm::scalar m_radius;
}



#endif // __COLLIDER_H__