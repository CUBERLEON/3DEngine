#include "Material.h"

#include "../core/ResourcesMgr.h"
#include "Texture.h"
#include "../core/Debug.h"

Material::Material()
: m_specularColor(glm::vec3(1, 1, 1)), m_specularPower(1.),
  m_diffuseTexture(ResourcesMgr::texture("models/default_diffuse.png")),
  m_normalTexture(ResourcesMgr::texture("models/default_normal.png")),
  m_specularTexture(ResourcesMgr::texture("models/default_specular.png")),
  m_displacementTexture(ResourcesMgr::texture("models/default_displacement.png")),
  m_alphaTexture(ResourcesMgr::texture("models/default_alpha.png"))
{}

Material::~Material()
{}