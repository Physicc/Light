#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "SceneObject.hpp"

class Skybox : public SceneObject
{
    public:
        Skybox();
        void render() override;

    private:
        std::shared_ptr<Light::Cubemap> m_cubemap;
};

#endif //__SKYBOX_H__
