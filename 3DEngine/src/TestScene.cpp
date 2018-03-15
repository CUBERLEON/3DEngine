#include "TestScene.h"

#include <vector>
#include <cmath>
#include "3DEngine.h"

TestScene::TestScene()
{}

TestScene::~TestScene()
{}

void TestScene::init() 
{
	//meshes
// 	ResourcesMgr::node("sponza")->getTransform()->setScale(3, 3, 3);
// 	ResourcesMgr::node("sponza")->getTransform()->setScale(0.3, 0.3, 0.3);
//	ResourcesMgr::node("sponza")->getTransform()->setScale(0.2, 0.2, 0.2);
 	ResourcesMgr::node("sponza")->getTransform()->setScale(0.03, 0.03, 0.03);
// 	ResourcesMgr::node("sponza")->getTransform()->setScale(0.003, 0.003, 0.003);
 	getRoot()->attach(ResourcesMgr::node("sponza")->addComponent(ResourcesMgr::createComponent<Entity>("sponza")->addMeshes(ResourcesMgr::meshArray("models/crytek/sponza.obj"))));
// 	getRoot()->attach(ResourcesMgr::node("sponza")->addComponent(ResourcesMgr::createComponent<Entity>("sponza")->addMeshes(ResourcesMgr::meshArray("models/rungholt/house.obj"))));
//	getRoot()->attach(ResourcesMgr::node("sponza")->addComponent(ResourcesMgr::createComponent<Entity>("sponza")->addMeshes(ResourcesMgr::meshArray("models/rungholt/rungholt.obj"))));
// 	getRoot()->attach(ResourcesMgr::node("sponza")->addComponent(ResourcesMgr::createComponent<Entity>("sponza")->addMeshes(ResourcesMgr::meshArray("models/lost_empire/lost_empire.obj"))));
// 	ResourcesMgr::material("models/crytek/sponza.obj>fabric_c")->setDiffuseTexture(ResourcesMgr::texture("models/crate.jpg"));

	getRoot()->attach(ResourcesMgr::node("plane")->addComponent(ResourcesMgr::createComponent<Entity>("plane")->addMeshes(ResourcesMgr::meshArray("models/plane.obj"))));
	getRoot()->attach(ResourcesMgr::node("plane1")->addComponent(ResourcesMgr::createComponent<Entity>("plane1")->addMeshes(ResourcesMgr::meshArray("models/plane.obj"))));
	getRoot()->attach(ResourcesMgr::node("sphere")->addComponent(ResourcesMgr::createComponent<Entity>("sphere")->addMeshes(ResourcesMgr::meshArray("models/earth/earth.obj"))));
	getRoot()->attach(ResourcesMgr::node("cube")->addComponent(ResourcesMgr::createComponent<Entity>("cube")->addMeshes(ResourcesMgr::meshArray("models/cube.obj"))));
// 	getRoot()->attach(ResourcesMgr::node("barrel")->addComponent(ResourcesMgr::createComponent<Entity>("barrel")->addMeshes(ResourcesMgr::meshArray("models/barrels/metal_barrel.obj"))));
	ResourcesMgr::node("plane")->getTransform()->setScale(2, 2, 2)->setRotation(glm::radians(90.), 0, 0)->setPosition(-5, 2, 0);
	ResourcesMgr::node("plane1")->getTransform()->setScale(8, 8, 8);
	ResourcesMgr::node("sphere")->getTransform()->setPosition(0, 1, 0)->setRotation(glm::radians(90.), 0, 0);
	ResourcesMgr::node("cube")->getTransform()->setPosition(3, 1, 0)->setScale(2, 2, 2);
// 	ResourcesMgr::node("barrel")->getTransform()->setPosition(0, 0, 4)->setScale(0.05, 0.05, 0.05)->setRotation(0, glm::radians(90.), 0);
// 	ResourcesMgr::material("default")->setDiffuseTexture(ResourcesMgr::texture("models/barrels/diffuse_rust.jpg"))->setNormalTexture(ResourcesMgr::texture("models/barrels/normal_no_bumps.jpg"));
	ResourcesMgr::material("default")->setDiffuseTexture(ResourcesMgr::texture("models/brickwall.jpg"))->setNormalTexture(ResourcesMgr::texture("models/brickwall_normal.jpg"));
// 	ResourcesMgr::node("plane")->attach({ ResourcesMgr::node("sphere"), ResourcesMgr::node("cube") });

// 	getRoot()->attach(ResourcesMgr::node("nanosuit")->addComponent(ResourcesMgr::createComponent<Entity>("nanosuit")->addMeshes(ResourcesMgr::meshArray("models/nanosuit2/nanosuit.obj"))));
	getRoot()->attach(ResourcesMgr::node("nanosuit")->addComponent(ResourcesMgr::createComponent<Entity>("nanosuit")->addMeshes(ResourcesMgr::meshArray("models/cyborg/cyborg.obj"))));
	ResourcesMgr::node("nanosuit")->getTransform()->setPosition(0, 0, -2)->setScale(1, 1, 1);// ->setRotation(0, glm::radians(180.), 0);

	//lights
	std::vector< std::shared_ptr<Node> > lightNodes;
	lightNodes.push_back(ResourcesMgr::node("directional1")->addComponent(ResourcesMgr::createComponent<DirectionalLight>("directional1", glm::vec3(1, 1, 1), 1.f)));
	ResourcesMgr::node("directional1")->getTransform()->setRotation(glm::radians(-70.), 0, 0);
// 	lightNodes.push_back(ResourcesMgr::node("point1")->addComponent(ResourcesMgr::createComponent<PointLight>("point1", glm::vec3(1, 1, 1), 10.0f, Attenuation(0, 1, 0))));
// 	ResourcesMgr::node("point1")->getTransform()->setPosition(glm::vec3(0.0f, 20.0f, 0.0f));
// 	lightNodes.push_back(ResourcesManager::node("point2")->addComponent(ResourcesManager::createComponent<PointLight>("point2", glm::vec3(0, 1, 1), 3.0f, Attenuation(0, 0, 1))));
// 	ResourcesManager::node("point2")->getTransform()->setPosition(glm::vec3(0, 3, 2));
// 	lightNodes.push_back(ResourcesManager::node("spot1")->addComponent(ResourcesManager::createComponent<SpotLight>("spot1", glm::vec3(1, 1, 1), 2.0f, Attenuation(0.5f, 0.15f, 0), cos(glm::radians(15.)))));
// 	ResourcesManager::node("spot1")->getTransform()->setPosition(glm::vec3(0, 3, -2))->setRotation(glm::angleAxis(glm::radians(-150.f), glm::vec3(1, 0, 0)));
// 	lightNodes.push_back(ResourcesManager::node("spot2")->addComponent(ResourcesManager::createComponent<SpotLight>("spot2", glm::vec3(1, 1, 1), 2.0f, Attenuation(0.5f, 0.15f, 0), cos(glm::radians(15.)))));
// 	ResourcesManager::node("spot2")->getTransform()->setPosition(glm::vec3(10, 3, -4))->setRotation(glm::angleAxis(glm::radians(90.f), glm::vec3(1, 0, 1)));
	getRoot()->attach(lightNodes);

	//camera
	ResourcesMgr::node("camera1")->getTransform()->setPosition(glm::vec3(5, 5, 5))->setRotation(glm::quat(glm::vec3(glm::radians(-45.), 0, 0)));
	getRoot()->attach(ResourcesMgr::node("camera1")->addComponent(ResourcesMgr::createComponent<PerspectiveCamera>("camera1", (float)glm::radians(60.), getWindow()->getAspectRatio(), 0.1f, 1000.0f)));
	
	ResourcesMgr::node("camera2")->getTransform()->setPosition(glm::vec3(20, 10, 0))->setRotation(glm::quat(glm::vec3(0, glm::radians(90.), glm::radians(10.))));
	getRoot()->attach(ResourcesMgr::node("camera2")->addComponent(ResourcesMgr::createComponent<PerspectiveCamera>("camera2", (float)glm::radians(60.), getWindow()->getAspectRatio(), 0.1f, 1000.0f)));
// 	ResourcesMgr::node("camera2")->addComponent(ResourcesMgr::createComponent<SpotLight>("spot2", glm::vec3(1, 1, 1), 2.0f, Attenuation(0.5f, 0.15f, 0), cos(glm::radians(30.))));
	ResourcesMgr::node("cube")->addComponent(ResourcesMgr::getComponent<PerspectiveCamera>("camera2"));

	ResourcesMgr::getComponent<PerspectiveCamera>("camera1")->activate();

	//skybox
	setSkybox(ResourcesMgr::skybox("test1"));
}

void TestScene::update(float time)
{
	Input* i = getWindow()->getInput();
	if (i->isKeyPressed(Input::KEY_C))
	{
		if (ResourcesMgr::getComponent<PerspectiveCamera>("camera1")->isActive())
			ResourcesMgr::getComponent<PerspectiveCamera>("camera2")->activate();
		else
			ResourcesMgr::getComponent<PerspectiveCamera>("camera1")->activate();
	}
	if (i->isKeyPressed(Input::KEY_F))
	{
		if (!getRoot()->detach(ResourcesMgr::node("sponza")))
			getRoot()->attach(ResourcesMgr::node("sponza"));
	}
	if (i->isKeyPressed(Input::KEY_R))
	{
		ResourcesMgr::eraseUnused();
	}
	if (i->isKeyPressed(Input::KEY_F1))
	{
		Debug::setShader(!Debug::isEnabledShader());
	}
	if (i->getKey(Input::KEY_LEFT))
	{
		ResourcesMgr::node("directional1")->getTransform()->rotate(glm::quat(glm::vec3(time/4, 0, 0)));
	}
	if (i->getKey(Input::KEY_RIGHT))
	{
		ResourcesMgr::node("directional1")->getTransform()->rotate(glm::quat(glm::vec3(-time/4, 0, 0)));
	}
	if (i->getKey(Input::KEY_UP))
	{
		ResourcesMgr::node("directional1")->getTransform()->rotate(glm::quat(glm::vec3(0, 0, time / 4)));
	}
	if (i->getKey(Input::KEY_DOWN))
	{
		ResourcesMgr::node("directional1")->getTransform()->rotate(glm::quat(glm::vec3(0, 0, -time / 4)));
	}
	if (i->isKeyPressed(Input::KEY_L))
	{
		if (!ResourcesMgr::node("directional1")->removeComponent(ResourcesMgr::getComponent<DirectionalLight>("directional1")))
			ResourcesMgr::node("directional1")->addComponent(ResourcesMgr::getComponent<DirectionalLight>("directional1"));
	}
	if (i->isKeyPressed(Input::KEY_B))
	{
		if (!getSkybox())
			setSkybox(ResourcesMgr::skybox("test1"));
		else
			setSkybox(nullptr);
	}

	ResourcesMgr::node("nanosuit")->getTransform()->rotate(glm::quat(glm::vec3(0, time/2, 0)));
	ResourcesMgr::node("sphere")->getTransform()->rotate(glm::quat(glm::vec3(0, time/3, 0)));
}