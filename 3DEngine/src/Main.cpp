#include "core/Core.h"
#include "core/Debug.h"
#include "TestScene.h"

int main()
{
	try
	{
		Core* engine = new Core(new TestScene());
// 		engine->lockFPS(60);
// 		engine->createWindow(1366, 768, "Test", true);
// 		engine->createWindow(800, 600, "Test");
// 		engine->createWindow(1920, 1080, "Test", true);
		engine->createWindow(800, 600, "Test");
		engine->start();
	}
	catch (const std::exception& e)
	{
		Debug::fatalError("Unhandled exception. " + std::string(e.what()));
	}
	return 0;
}