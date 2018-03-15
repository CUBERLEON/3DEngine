#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "../Light.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight(const std::string& name, const glm::vec3& color, float intensity);
    ~DirectionalLight();

    glm::vec3 getDirection();
protected:
private:
    glm::mat4 getShadowTransform() override;
};

#endif // DIRECTIONAL_LIGHT_H
