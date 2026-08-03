#include "ApplicationManager.h"

#include <algorithm>
#include <iostream>

#include "../Math/Includes/Transform.h"
#include "DeltaTime.h"
#include "Model.h"

ApplicationManager::ApplicationManager(std::filesystem::path&& path)
    :m_window(Window(SDL_WINDOW_RESIZABLE, "3D Renderer", (path / "Assets" / "Images" / "cubeIcon.png"), 1280, 720)),
    m_draw(m_window),
    m_running(true),
    m_backCull(true),
    m_meshMode(MODE_FILL_TEXTURE),
    m_path(std::move(path))

{
    m_camera.position = math::vec3(0.0f, 0.0f, 3.0f);
    m_camera.forward = math::vec3(0.0f, 0.0f, -1.0f);
    m_camera.worldUp = math::vec3(0.0f, 1.0f, 0.0f);
}


void ApplicationManager::Setup()
{
    if (!m_window.InitializeWindow(m_window.getWindowFlags()))
    {
        SDL_Log("Error with initialization of the window");
    }

    m_draw.InitRenderer();
    InitSTBImage();

    SDL_SetWindowRelativeMouseMode(m_window.getSDLWindow(), true);

    Mesh cubeMesh, chickenMesh, appleMesh, orangeMesh, carMesh, snowmanMesh;

    CreateMeshOBJ(cubeMesh, (m_path / "Assets" / "Models" / "cube.obj"));
    CreateMeshOBJ(chickenMesh, (m_path / "Assets" / "Models" / "chicken.obj"));
    CreateMeshOBJ(appleMesh, (m_path / "Assets" / "Models" / "apple.obj"));
    CreateMeshOBJ(orangeMesh, (m_path / "Assets" / "Models" / "orange.obj"));
    CreateMeshOBJ(carMesh, (m_path / "Assets" / "Models" / "car.obj"));
    CreateMeshOBJ(snowmanMesh, (m_path / "Assets" / "Models" / "Snowman.obj"));

    cubeMesh.AddTexture((m_path / "Assets" / "Images" / "luckyBlock.png"));
    chickenMesh.AddTexture((m_path / "Assets" / "Images" / "ChickenTexture.jpeg"));
    appleMesh.AddTexture((m_path / "Assets" / "Images" / "appleTexture.png"));
    orangeMesh.AddTexture((m_path / "Assets" / "Images" / "orangeTexture.jpeg"));
    carMesh.AddTexture((m_path / "Assets" / "Images" / "CarTexture.jpeg"));
    snowmanMesh.AddTexture((m_path / "Assets" / "Images" / "SnowmanTexture.png"));



    m_gameObjects.emplace_back(std::move(cubeMesh),
    math::vec3(0.5f, 0.5f, 0.5f),
    math::vec3(0.0f, 0.0f, 0.0f),
    math::vec3(0.0f, 0.0f, -7.0f));


    m_gameObjects.emplace_back(std::move(chickenMesh),
    math::vec3(0.5f, 0.5f, 0.5f),
    math::vec3(0.0f, 0.0f, 0.0f),
    math::vec3(-0.25f, -0.25f, -1.5f));


    m_gameObjects.emplace_back(std::move(appleMesh),
    math::vec3(1.25f, 1.25f, 1.25f),
    math::vec3(0.0f, 0.0f, 0.0f),
    math::vec3(2.25f, -0.25f, -0.5f));


    m_gameObjects.emplace_back(std::move(orangeMesh),
    math::vec3(0.025f, 0.025f, 0.025f),
    math::vec3(0.0f, math::toRadians(-90.0f), 0.0f),
    math::vec3(2, 0, -1.5f));


    m_gameObjects.emplace_back(std::move(carMesh),
    math::vec3(1.0f, 1.0f, 1.0f),
    math::vec3(0.0f, math::toRadians(90.0f), 0.0f),
    math::vec3(-5.0f, 0.0f, -5.0f));


    m_gameObjects.emplace_back(std::move(snowmanMesh),
        math::vec3(0.5f, 0.5f, 0.5f),
        math::vec3(0.0f, 0.0f, 0.0f),
        math::vec3(3.0f, -0.5f, -3.0f));

    //Perspective projection matrix
    float aspectRatio =  static_cast<float> (m_draw.getWindow().getHeight()) /  static_cast<float> (m_draw.getWindow().getWidth());
    m_projectionMat = math::PerspectiveProjectionMat4(math::toRadians(90.0f), aspectRatio, 0.1f, 100.0f);
}


void ApplicationManager::HandleInputs()
{
    bool mouseLocked = SDL_GetWindowRelativeMouseMode(m_window.getSDLWindow());

    MovementInput();

    if (mouseLocked)
    {
        MouseInput();
    }


    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_EVENT_QUIT:
                m_running = false;
                break;

                //If someone alt tabs from the application release the mouse, if they come back on hide it and lock it in the center
            case SDL_EVENT_WINDOW_FOCUS_LOST:
                SDL_SetWindowRelativeMouseMode(m_window.getSDLWindow(), false);
                break;

            case SDL_EVENT_WINDOW_FOCUS_GAINED:
                SDL_SetWindowRelativeMouseMode(m_window.getSDLWindow(), true);
                break;

            case SDL_EVENT_WINDOW_RESIZED:
            {
                m_window.ChangeSize(event.window.data1, event.window.data2);
                m_draw.InitRenderer();

                //Perspective projection matrix
                float aspectRatio =  static_cast<float> (m_draw.getWindow().getHeight()) /  static_cast<float> (m_draw.getWindow().getWidth());
                m_projectionMat = math::PerspectiveProjectionMat4(math::toRadians(90.0f), aspectRatio, 0.1f, 100.0f);

                break;
            }

            case SDL_EVENT_KEY_DOWN:

                switch (event.key.key)
                {
                    case SDLK_ESCAPE:
                        m_running = false;
                        break;

                    case SDLK_1:
                        m_meshMode = MODE_WIREFRAME_VERTEX;
                        break;

                    case SDLK_2:
                        m_meshMode = MODE_WIREFRAME;
                        break;

                    case SDLK_3:
                        m_meshMode = MODE_FILL;
                        break;

                    case SDLK_4:
                        m_meshMode = MODE_FILL_WIREFRAME;
                        break;

                    case SDLK_5:
                        m_meshMode = MODE_FILL_TEXTURE;
                        break;

                    case SDLK_6:
                        m_meshMode = MODE_WIREFRAME_TEXTURE;
                        break;

                    case SDLK_C:
                        m_backCull = BACKFACE_CULL_ENABLED;
                        break;

                    case SDLK_V:
                        m_backCull = BACKFACE_CULL_DISABLED;
                        break;

                     case SDLK_TAB:
                        //A bit getting xor by 1 will always flip its value
                        SDL_SetWindowRelativeMouseMode(m_window.getSDLWindow(), mouseLocked ^ 1);
                        break;
                }
        }
    }
}

void ApplicationManager::Update()
{
    UpdateDeltaTime();
    m_camera.UpdateDirection();


    m_gameObjects[0].m_rotation += math::vec3(0.5f, 0.5f, 0.5f) * deltaTime;
    m_gameObjects[4].m_position.z = m_moveVar - 5.0f;
    m_gameObjects[5].m_rotation.y += 0.75f * deltaTime;

    math::vec3 target = m_camera.forward + m_camera.position;
    m_viewMat = math::LookAtMatrix(m_camera.position, target, math::vec3(0.0f, 1.0f, 0.0f));


    for (size_t i = 0; i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i].SetMeshMatrices(m_viewMat, m_projectionMat);
    }

    if (m_moveVar >= 4.0f)
    {
        m_incrementMoveVar = -2.0f;
    }

    else if (m_moveVar <= -4.0f)
    {
        m_incrementMoveVar = 2.0f;
    }



    m_moveVar += m_incrementMoveVar * deltaTime;
}

void ApplicationManager::Render()
{
    m_draw.ClearFrameBuffer(0x000000FF);
    m_draw.ClearDepthBuffer();


    for (size_t i = 0; i < m_gameObjects.size(); i++)
    {
        m_gameObjects[i].RenderObject(m_draw, m_backCull, m_meshMode);
    }

    m_draw.RenderFrameBuffer();

    SDL_RenderPresent(m_window.getRenderer());
}


void ApplicationManager::MovementInput()
{
    SDL_PumpEvents();

    const bool* keyStates = SDL_GetKeyboardState(NULL);

    if (keyStates[SDL_SCANCODE_W])
    {
        m_camera.position = m_camera.position + m_camera.forward * 5.0f * deltaTime;
    }

    if (keyStates[SDL_SCANCODE_S])
    {
        m_camera.position = m_camera.position - m_camera.forward * 5.0f * deltaTime;
    }

    if (keyStates[SDL_SCANCODE_A])
    {
        m_camera.position = m_camera.position - m_camera.right * 5.0f * deltaTime;
    }

    if (keyStates[SDL_SCANCODE_D])
    {
        m_camera.position = m_camera.position + m_camera.right * 5.0f * deltaTime;
    }

    if (keyStates[SDL_SCANCODE_Q])
    {
        m_camera.position = m_camera.position - m_camera.worldUp * 5.0f * deltaTime;
    }

    if (keyStates[SDL_SCANCODE_E])
    {
        m_camera.position = m_camera.position + m_camera.worldUp * 5.0f * deltaTime;
    }
}

void ApplicationManager::MouseInput()
{
    float deltaMouseX;
    float deltaMouseY;

    SDL_GetRelativeMouseState(&deltaMouseX, &deltaMouseY);

    //Y of mouse is normally inversed
    m_camera.yaw += deltaMouseX / 10.0f;
    m_camera.pitch -= deltaMouseY / 10.0f;


    m_camera.pitch = std::clamp(m_camera.pitch, -89.0f, 89.0f);
}


bool ApplicationManager::isRunning() const
{
    return m_running;
}



