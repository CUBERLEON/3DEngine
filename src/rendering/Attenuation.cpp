#include "Attenuation.h"

#include "../core/Debug.h"

Attenuation::Attenuation(float constant, float linear, float exponent) :
glm::vec3(constant, linear, exponent)
{}

Attenuation::~Attenuation()
{}
