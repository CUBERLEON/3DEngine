#pragma once

#include <glm\glm.hpp>

class Attenuation : public glm::vec3
{
public:
	Attenuation(float constant, float linear, float exponent);
	~Attenuation();

	inline float getConstant() const { return x; }
	inline void setConstant(float constantAttenuation) { x = constantAttenuation; }

	inline float getLinear() const { return y; }
	inline void setLinear(float linearAttenuation) { y = linearAttenuation; }

	inline float getExponent() const { return z; }
	inline void setExponent(float exponentAttenuation) { z = exponentAttenuation; }
protected:
private:
};

