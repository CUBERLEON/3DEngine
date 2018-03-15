#ifndef SCENE_H
#define SCENE_H

#include <memory>

class Node;
class Renderer;
class Core;
class Window;
class Skybox;

class Scene
{
    friend class Core;
public:
    Scene();
    ~Scene();

    std::shared_ptr<Node>& getRoot();
protected:
    Window* getWindow();
    const std::shared_ptr<Skybox>& getSkybox() const;

    void setSkybox(const std::shared_ptr<Skybox>& skybox);
private:
    std::shared_ptr<Node> m_root;

    Core* m_core;
    void setCore(Core* core);

    virtual void init() = 0;
    virtual void update(float time) = 0;
    
    void initBase();
    void updateBase(float time);
    void render(Renderer& renderingEngine);
};

#endif // SCENE_H
