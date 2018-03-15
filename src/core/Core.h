#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include <string>

class Scene;
class Renderer;
class Window;

class Core 
{
public:
    Core(Scene* game);
    ~Core();

    void createWindow(int width, int height, const std::string& title, bool fullscreen = false);

    void start();
    void stop();

    inline float getFpsLimit() const { return m_fpsLimit; }
    inline bool isFPSLocked() const { return m_isfpsLocked; }
    inline void lockFPS(float val) { m_fpsLimit = val; m_isfpsLocked = true; }
    inline void unlockFPS() { m_isfpsLocked = false; }

    Renderer* getRenderingEngine() const;
    Window* getWindow() const;
protected:
private:
    float m_fpsLimit;
    bool m_isfpsLocked;

    Renderer* m_renderingEngine;
    Window* m_window;
    Scene* m_scene;

    bool m_isRunning;

    void run();
};
#endif // CORE_ENGINE_H