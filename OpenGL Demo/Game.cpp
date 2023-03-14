#include <numbers>
#include "Game.h"
#include "ControllableCamera.h"
#include <glm/gtc/type_ptr.hpp>
#include "Time.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Model.h"

std::unique_ptr<Scene> CreateScene(const Window& window);

Game::Game(int width, int height, const std::string& title) :
    m_window(Window(width, height, title)),
    m_camera(std::make_shared<ControllableCamera>(glm::vec3(0.0f), -1.0f / 3.0f * std::numbers::pi))
{
    m_window.SetBackgroundColor(glm::vec4 { 0.0f, 0.0f, 0.0f, 1.0f });
}

Game& Game::GetInstance()
{
    static Game instance = Game(1024, 768, "Hello World!");
    return instance;
}

void Game::Update()
{
    /*SpotLight& flashLight = *(m_scene->GetSpotLights()[1]);

    const glm::vec3 offset = { 0.0f, -0.3f, 0.0f };
    flashLight.SetPosition(m_camera->GetPosition() - offset);
    flashLight.SetDirection(m_camera->GetOrientation());*/
}

void Game::Run()
{
    Game& game = GetInstance();
    game.Initialize();

    while (!game.m_window.IsClosed())
    {
        if (game.m_scene)
        {
            Time::Update();
            
            game.Update();
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

void averageNormals(size_t stride, size_t offset,
                    GLfloat* vertices, size_t vertexCount,
                    unsigned int* indices, size_t indexCount)
{
    for (size_t i = 0; i < indexCount; i += 3)
    {
        unsigned int in0 = indices[i] * stride;
        unsigned int in1 = indices[i + 1] * stride;
        unsigned int in2 = indices[i + 2] * stride;

        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        in0 += offset; in1 += offset; in2 += offset;
        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;   
    }

    for (size_t i = 0; i < vertexCount; i++)
    {
        GLfloat* normalData = vertices + i * stride + offset;
        glm::vec3 normal = *reinterpret_cast<glm::vec3*>(normalData);

        normal = glm::normalize(normal);
        
        memcpy(normalData, glm::value_ptr(normal), sizeof(normal));
    }
}

const size_t STRIDE = 8;
const size_t NORMAL_OFFSET = 5;

std::shared_ptr<Mesh> CreateFloor()
{
    GLfloat vertices[] = {
        -10.0f, 0.0f, -10.0f,   0.0f,  0.0f,    0.0f, -1.0f, 0.0f,
         10.0f, 0.0f, -10.0f,   10.0f, 0.0f,    0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f,  10.0f,   0.0f,  10.0f,   0.0f, -1.0f, 0.0f,
         10.0f, 0.0f,  10.0f,   10.0f, 10.0f,   0.0f, -1.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 2, 1,
        1, 2, 3
    };
    
    constexpr size_t VERTEX_COUNT = sizeof(vertices) / sizeof(GLfloat) / STRIDE;
    constexpr GLsizei INDEX_COUNT = sizeof(indices) / sizeof(unsigned int);

    return std::make_shared<Mesh>(vertices, VERTEX_COUNT * STRIDE, indices, INDEX_COUNT);
}

std::shared_ptr<Mesh> CreatePyramid()
{
    GLfloat vertices[] = {
    //    x      y      z        u     v         nx    ny    nz
        -1.0f, -1.0f, -0.6f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
         0.0f, -1.0f,  1.0f,    0.5f, 0.0f,     0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.6f,    1.0f, 0.0f,     0.0f, 0.0f, 0.0f,
         0.0f,  1.0f,  0.0f,    0.5f, 1.0f,     0.0f, 0.0f, 0.0f,
    };

    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
	};
    

    constexpr size_t VERTEX_COUNT = sizeof(vertices) / sizeof(GLfloat) / STRIDE;
    constexpr GLsizei INDEX_COUNT = sizeof(indices) / sizeof(unsigned int);

    averageNormals(STRIDE, NORMAL_OFFSET, vertices, VERTEX_COUNT, indices, INDEX_COUNT);
    
    return std::make_shared<Mesh>(vertices, VERTEX_COUNT * STRIDE, indices, INDEX_COUNT);
}

std::unique_ptr<Scene> CreateScene(const Window& window)
{
    std::unique_ptr<Scene> scene = std::make_unique<Scene>(window);
    
    std::shared_ptr<Shader> shader = Shader::FromFiles("Resources/Shaders/vertex.glsl", "Resources/Shaders/fragment.glsl");
    scene->UseShader(shader);
    
    std::shared_ptr<Renderable> pyramid = CreatePyramid();
    std::shared_ptr<Renderable> floor = CreateFloor();

    std::shared_ptr<Material> shinyMaterial = std::make_shared<Material>(1.0f, 256.0f);
    std::shared_ptr<Material> dullMaterial = std::make_shared<Material>(0.3f, 4.0f);

    std::shared_ptr<Texture> brickTexture = std::make_shared<Texture>("Resources/images/brick.png", true);
    std::shared_ptr<Texture> dirtTexture = std::make_shared<Texture>("Resources/images/dirt.png", true);
    std::shared_ptr<Texture> plainTexture = std::make_shared<Texture>("Resources/images/plain.png", true);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3 { 0.0f, 0.0f, -2.5f });
    std::shared_ptr<GameObject> shinyBrickPyramid = std::make_shared<GameObject>(pyramid, model, brickTexture, shinyMaterial);
    scene->AddObject(shinyBrickPyramid);

    model = glm::translate(glm::mat4(1.0f), glm::vec3 { 0.0f, 4.0f, -2.5f });
    std::shared_ptr<GameObject> dullDirtPyramid = std::make_shared<GameObject>(pyramid, model, dirtTexture, dullMaterial);
    scene->AddObject(dullDirtPyramid);
    
    model = glm::translate(glm::mat4(1.0f), glm::vec3 { 0.0f, -2.0f, 0.0f });
    std::shared_ptr<GameObject> floorObject = std::make_shared<GameObject>(floor, model, dirtTexture, shinyMaterial);
    scene->AddObject(floorObject);
    
    glm::vec3 white = { 1.0f, 1.0f, 1.0f };
    glm::vec3 red   = { 1.0f, 0.0f, 0.0f };
    glm::vec3 green = { 0.0f, 1.0f, 0.0f };
    glm::vec3 blue  = { 0.0f, 0.0f, 1.0f };

    glm::vec3 lightDirection = { 0.0f, 0.0f, -1.0f };
    std::unique_ptr<DirectionalLight> directionalLight = std::make_unique<DirectionalLight>(lightDirection, white, 0.3f, 0.6f);
    
    std::unique_ptr<PointLight> greenLight = std::make_unique<PointLight>(
        glm::vec3 { 0.0f, 0.0f, 0.0f },
        glm::vec3 { 0.1f, 0.2f, 0.3f, },
        blue, 0.0, 0.1f
    );

    std::unique_ptr<PointLight> blueLight = std::make_unique<PointLight>(
        glm::vec3 { -4.0f, 2.0f, 0.0f },
        glm::vec3 { 0.1f, 0.1f, 0.3f },
        green, 0.0f, 0.1f
    );

    std::unique_ptr<SpotLight> down = std::make_unique<SpotLight>(
        glm::vec3 { 0.0f, -1.0f, 0.0f }, 20.0f,
        glm::vec3 { 0.0f, 0.0f, 0.0f },
        glm::vec3 { 0.0f, 0.0f, 1.0f },
        white, 0.0f, 1.0f
    );
    
    std::unique_ptr<SpotLight> flashLight = std::make_unique<SpotLight>(
        glm::vec3 { -5.0f, -1.0f, 0.0f, }, 20.0f,
        glm::vec3 { 0.0f, 1.5f, 0.0f },
        glm::vec3 { 0.0f, 0.0f, 1.0f },
        white, 0.0f, 2.0f
    );

    scene->AddDirectionalLight(directionalLight);
    scene->AddPointLight(greenLight);
    scene->AddPointLight(blueLight);
    scene->AddSpotLight(down);
    scene->AddSpotLight(flashLight);
    
    model = glm::translate(glm::mat4(1.0f), glm::vec3 { -7.0f, 0.0f, 10.0f });
    model = glm::scale(model, glm::vec3(0.006f));
    std::shared_ptr<Renderable> xwingModel = std::make_shared<Model>("Resources/Models/x-wing.obj");
    std::shared_ptr<GameObject> xwing = std::make_shared<GameObject>(xwingModel, model);
    scene->AddObject(xwing);

    model = glm::translate(glm::mat4(1.0f), glm::vec3 { -3.0f, 2.0f, 0.0f });
    model = glm::rotate(model, (float) -std::numbers::pi / 2, glm::vec3 { 1.0f, 0.0f, 0.0f });
    model = glm::scale(model, glm::vec3(0.4f));
    std::shared_ptr<Renderable> heliModel = std::make_shared<Model>("Resources/Models/uh60.obj");
    std::shared_ptr<GameObject> heli = std::make_shared<GameObject>(heliModel, model);
    scene->AddObject(heli);

    return scene;
}
