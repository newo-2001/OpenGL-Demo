#pragma once

#include <vector>
#include <memory>

#include "glm/mat4x4.hpp"

#include "Shader.h"
#include "Mesh.h"
#include "Window.h"
#include "Scene.h"
#include "Camera.h"

class Game
{
public:
    static Game& GetInstance();
    static void Run();

    void Update();

    Window& GetWindow() { return m_window; }
    std::weak_ptr<Camera> GetCamera() { return std::weak_ptr<Camera>(m_camera); }
    std::weak_ptr<Scene> GetActiveScene() { return std::weak_ptr<Scene>(m_scene); }

    void ChangeScene(std::shared_ptr<Scene>& scene) { m_scene = scene; }
private:
    Window m_window;
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<Scene> m_scene;
    
    Game(int width, int height, const std::string& title);
    void Initialize();

    bool m_flash = false;
};
