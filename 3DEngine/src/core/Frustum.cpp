#include "Frustum.h"

#include "AABox.h"

Frustum::Frustum()
{
    m_planes = new Plane[6]; //creating array of 6 frustum's clipping planes (sides)
}

Frustum::~Frustum()
{
    delete[] m_planes; //deleting planes
}

void Frustum::setViewProjectionMatrix(const glm::mat4& vpm) //computes frustum's clipping planes from view-projection matrix (normal of each plane points inside of the frustum)
{
    //left
    m_planes[0].a = vpm[0][3] + vpm[0][0];
    m_planes[0].b = vpm[1][3] + vpm[1][0];
    m_planes[0].c = vpm[2][3] + vpm[2][0];
    m_planes[0].d = vpm[3][3] + vpm[3][0];
    //right
    m_planes[1].a = vpm[0][3] - vpm[0][0];
    m_planes[1].b = vpm[1][3] - vpm[1][0];
    m_planes[1].c = vpm[2][3] - vpm[2][0];
    m_planes[1].d = vpm[3][3] - vpm[3][0];
    //top
    m_planes[2].a = vpm[0][3] - vpm[0][1];
    m_planes[2].b = vpm[1][3] - vpm[1][1];
    m_planes[2].c = vpm[2][3] - vpm[2][1];
    m_planes[2].d = vpm[3][3] - vpm[3][1];
    //bottom
    m_planes[3].a = vpm[0][3] + vpm[0][1];
    m_planes[3].b = vpm[1][3] + vpm[1][1];
    m_planes[3].c = vpm[2][3] + vpm[2][1];
    m_planes[3].d = vpm[3][3] + vpm[3][1];
    //near
    m_planes[4].a = vpm[0][2];
    m_planes[4].b = vpm[1][2];
    m_planes[4].c = vpm[2][2];
    m_planes[4].d = vpm[3][2];
    //far
    m_planes[5].a = vpm[0][3] - vpm[0][2];
    m_planes[5].b = vpm[1][3] - vpm[1][2];
    m_planes[5].c = vpm[2][3] - vpm[2][2];
    m_planes[5].d = vpm[3][3] - vpm[3][2];

    for (int i = 0; i < 6; ++i) //loop through all frustum's planes
        m_planes[i].normalize(); //normalizing i-th plane
}

int Frustum::isBoxInside(const AABox& box) //checks if axis aligned bounding box is inside frustum
{
    int result = 2; //initializing result as inside
    for (int i = 0; i < 6; ++i) //loop through all frustum's planes
    {
        if (m_planes[i].distance(box.getPositiveVertex(m_planes[i].getNormal())) < 0) //computing signed distance to positive vertex (normal points inside of the frustum)
            return 0; //outside (positive vertex of the box is outside frustum's plane)
        else if (m_planes[i].distance(box.getNegativeVertex(m_planes[i].getNormal())) < 0)
            result = 1; //partly inside relatively i-th plane (positive vertex of the box is inside frustum's plane, but negative one outside), cannot be inside anymore
        //else both vertices inside of the frustum relatively i-th plane
    }
    return result; //box passed all tests for all planes (inside or partly inside)
}

void Frustum::Plane::normalize() //normalizes plane
{
    float t = sqrt(a*a + b*b + c*c); //computing length of plane's normal
    a /= t; b /= t; c /= t; d /= t; //normalizing
}

float Frustum::Plane::distance(glm::vec3 p) //computes signed distance from the plane to a point
{
    return a * p.x + b * p.y + c * p.z + d; //computing signed distance
}

glm::vec3 Frustum::Plane::getNormal() //returns plane's normal
{
    return glm::vec3(a, b, c);
}
