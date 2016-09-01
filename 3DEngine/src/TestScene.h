#pragma once

#include "core/Scene.h"

class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();

	void init() override;
	void update(float time) override;
};

