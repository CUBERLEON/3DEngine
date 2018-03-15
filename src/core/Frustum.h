/*Frustum class. Required for checking if AABox is inside camera's frustum or not (culling mechanism)*/

#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <glm/glm.hpp>
#include <cmath>

class AABox;

class Frustum
{
private:
    struct Plane
    {
        float a, b, c, d;

        void normalize(); //normalizes plane
        float distance(glm::vec3 p); //computes signed distance from the plane to a point
        glm::vec3 getNormal(); //return plane's normal
    };
public:
    Frustum(); //view-projection matrix
    ~Frustum(); //destructor

    void setViewProjectionMatrix(const glm::mat4& vpm); //computes frustum's clipping planes from view-projection matrix (normal of each plane points inside of the frustum)

    int isBoxInside(const AABox& box); //checks if axis aligned bounding box is inside frustum (0 - outside, 1 - partly inside, 2 - inside)
protected:
private:
    Plane* m_planes; //array of 6 frustum's clipping planes (0 - Left, 1 - Right, 2 - Top, 3 - Bottom, 4 - Near, 5 - Far sides)
};

#endif