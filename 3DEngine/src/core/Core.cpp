#include "Core.h"

#include <chrono>
#include <thread>

#include "../rendering/Renderer.h"
#include "../rendering/Window.h"
#include "Scene.h"
#include "Input.h"
#include "ResourcesMgr.h"
#include "Debug.h"
#include "Time.h"

Core::Core(Scene* scene) : m_isfpsLocked(false), m_scene(scene), m_isRunning(false)
{
	ResourcesMgr::setCore(this);
}

Core::~Core()
{}

void Core::createWindow(int width, int height, const std::string& title, bool fullscreen)
{
	m_window = new Window(width, height, title, fullscreen);
	m_renderingEngine = new Renderer(m_window);
}

void Core::start()
{
	if (m_isRunning)
		return;

	if (!m_window)
		Debug::fatalError("Trying to start Core when Window wasn't created");

	m_scene->setCore(this);
	m_scene->initBase();

	m_window->getInput()->setMousePosition(m_window->getCenter());

	run();
}

void Core::stop()
{
	if (!m_isRunning)
		return;

	m_isRunning = false;
}

Renderer* Core::getRenderingEngine() const
{
	if (m_renderingEngine == nullptr)
		Debug::warning("NULL returned while executing CoreEngine.getRenderingEngine()");
	return m_renderingEngine;
}

Window* Core::getWindow() const
{
	if (m_window == nullptr)
		Debug::warning("NULL returned while executing CoreEngine.getWindow()");
	return m_window;
}

void Core::run()
{
	m_isRunning = true;

	int frames = 0;
	double fpsTime = 0;
	double fpsUpdateTime = 0;
	double fpsRenderTime = 0;
	double fpsRefreshTime = 1.0;
	double stopwatch;

	double startTime = Time::getTime(), endTime;

	double frameTime = 1.0 / m_fpsLimit;
	double unprocessedTime = 0;

	while (m_isRunning)
	{
		if (m_window->isCloseRequested())
			stop();

		endTime = Time::getTime();
		double passedTime = endTime - startTime;
		startTime = endTime;

		unprocessedTime += passedTime;
		fpsTime += passedTime;

		stopwatch = Time::getTime();
		m_window->getInput()->update();
		m_scene->updateBase((float)passedTime);
		fpsUpdateTime += Time::getTime() - stopwatch;

		bool render = false;
		if (unprocessedTime >= frameTime)
		{
			unprocessedTime = 0;
			render = true;
		}

		if (fpsTime >= fpsRefreshTime)
		{
			double k = 1000. / (frames + 1e-10);
			Debug::info("%.2f ms (%.2f U, %.2f R)", k * fpsTime, k * fpsUpdateTime, k * fpsRenderTime);
			fpsTime = fpsUpdateTime = fpsRenderTime = 0;
			frames = 0;
		}

		if (render || !m_isfpsLocked)
		{
			stopwatch = Time::getTime();
			m_scene->render(*m_renderingEngine);
			fpsRenderTime += Time::getTime() - stopwatch;

			frames++;
		}
		else 
		{
// 			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}
	}
}