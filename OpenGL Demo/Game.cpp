#include "Game.h"
#include "ControllableCamera.h"

std::unique_ptr<Scene> CreateScene(const Window& window);

Game::Game(int width, int height, const std::string& title) :
    m_window(Window(width, height, title)),
    m_camera(std::make_shared<ControllableCamera>())
{
    m_window.SetBackgroundColor(glm::vec4 { 0.35f, 0.75f, 0.66f, 1.0f });
}

Game& Game::GetInstance()
{
    static Game instance = Game(800, 600, "Hello World!");
    return instance;
}

void Game::Run()
{
    Game& game = GetInstance();
    game.Initialize();

    while (!game.m_window.IsClosed())
    {
        if (game.m_scene)
        {
            game.m_scene->Render();
            game.m_camera->Update();
        }

        game.m_window.Update();
    }
}

void Game::Initialize()
{
    std::shared_ptr<Scene> scene = CreateScene(m_window);
    ChangeScene(scene);
}

std::unique_ptr<Scene> CreateScene(const Window& window)
{
    std::unique_ptr<Scene> scene = std::make_unique<Scene>(window);
    
    std::shared_ptr<Shader> shader = Shader::FromFiles("Resources/Shaders/vertex.glsl", "Resources/Shaders/fragment.glsl");
    scene->UseShader(shader);
    
    constexpr GLfloat vertices[] = {
    //    x     y    z       u    v
        -1.0, -1.0, 1.0,    0.0f, 0.0f,
         1.0, -1.0, 1.0,    1.0f, 0.0f,
         1.0,  1.0, 1.0,    1.0f, 1.0f,
        -1.0,  1.0, 1.0,    0.0f, 1.0f,

        -1.0, -1.0, -1.0,   0.0f, 0.0f,
         1.0, -1.0, -1.0,   1.0f, 0.0f,
         1.0,  1.0, -1.0,   1.0f, 1.0f,
        -1.0,  1.0, -1.0,   0.0f, 1.0f
    };

    constexpr unsigned int indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};
    
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertices, 8 * 5, indices, sizeof(indices) / sizeof(unsigned int));

    std::shared_ptr<Texture> texture = std::make_shared<Texture>("Resources/images/brick.png");
    mesh->ApplyTexture(texture);

    scene->AddMesh(mesh);

    return scene;
}
