#include "Renderer.h"

#include "Shader.h"
#include "Window.h"
#include "Skybox.h"
#include "../core/ResourcesMgr.h"
#include "../components/Camera.h"
#include "../components/Light.h"
#include "../core/Node.h"
#include "../core/Debug.h"
#include "Texture.h"

Renderer::Renderer(Window* window)
: m_window(window), m_shadowWidth(1024), m_shadowHeight(1024), m_skybox(nullptr)
{
	Debug::info("OpenGL version " + getOpenGLVersion());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//depth test
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	//blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

// 	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//shadows
	m_shadowHeight = m_shadowWidth = 1024;
	m_shadowMap = new Texture(GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, GL_REPEAT);
// 	glBindTexture(GL_TEXTURE_2D, m_shadowMap->getID());
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	m_shadowMap->setImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowWidth, m_shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
// 	m_shadowMap->set(0, GL_RGBA8, m_shadowWidth, m_shadowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	m_depthShader = ResourcesMgr::shader("shadow-map");

	glGenFramebuffers(1, &m_shadowMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap->getID(), 0);
// 	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_shadowMap->getID(), 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Debug::fatalError("Shadow map framebuffer is not complete");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//ambient light
	m_ambientColor = glm::vec3(0.15f);

	//debug
	m_debugShader = ResourcesMgr::shader("debug");

	m_skyboxShader = ResourcesMgr::shader("skybox");
}

Renderer::~Renderer()
{}

void Renderer::render(std::shared_ptr<Node>& root)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 	glBlendFunc(GL_ONE, GL_ONE);
// 	glEnable(GL_BLEND);

	glDisable(GL_BLEND);
	for (auto& light : m_lights)
	{
		m_activeLight = light;

// 		glCullFace(GL_FRONT);
// 		glDisable(GL_CULL_FACE);
		glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO);
		glViewport(0, 0, m_shadowWidth, m_shadowHeight);
		glClear(GL_DEPTH_BUFFER_BIT);
		renderAux(root, m_depthShader);
// 		glBindTexture(GL_TEXTURE_2D, m_shadowMap->getID());
// 		glGenerateMipmap(GL_TEXTURE_2D);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

// 		glCullFace(GL_BACK);
// 		glEnable(GL_CULL_FACE);
		glViewport(0, 0, m_window->getWidth(), m_window->getHeight());
		renderAux(root, light->getShader());

		glEnable(GL_BLEND);
	}
	glDisable(GL_BLEND);

	if (Debug::isEnabledShader())
		renderAux(root, m_debugShader);

	if (m_skybox != nullptr)
	{
		m_skyboxShader->bind();
		m_skyboxShader->updateCommon(*this);
		m_skybox->render();
	}

	m_window->refresh();
}

void Renderer::renderAux(std::shared_ptr<Node>& root, std::shared_ptr<Shader>& shader)
{
	shader->bind();
	shader->updateCommon(*this);
	root->render(shader, *this);
}

std::string Renderer::getOpenGLVersion()
{
	return std::string((char*)glGetString(GL_VERSION));
}

const std::shared_ptr<Camera>& Renderer::getActiveCamera() const
{
	if (m_activeCamera == nullptr)
		Debug::warning("No active camera in the scene graph. Returning nullptr");

	return m_activeCamera;
}

void Renderer::setCamera(const std::shared_ptr<Camera>& mainCamera)
{
	m_activeCamera = mainCamera;
}

void Renderer::setSkybox(const std::shared_ptr<Skybox>& skybox)
{
	m_skybox = skybox;
}

const std::shared_ptr<Light>& Renderer::getActiveLight() const
{
	if (m_activeCamera == nullptr)
		Debug::warning("No active light. Returning nullptr");

	return m_activeLight;
}

const std::shared_ptr<Skybox>& Renderer::getSkybox() const
{
	return m_skybox;
}

void Renderer::addLight(const std::shared_ptr<Light>& light)
{
	m_lights.insert(light);
}

bool Renderer::removeLight(const std::shared_ptr<Light>& light)
{
	auto it = m_lights.find(light);
	if (it != m_lights.end())
	{
		m_lights.erase(it);
		return true;
	}
	return false;
}
