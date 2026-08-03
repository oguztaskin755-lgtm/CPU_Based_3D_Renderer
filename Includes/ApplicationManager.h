#pragma once

#include <vector>
#include <filesystem>

#include "Draw.h"
#include "Mesh.h"
#include "Camera.h"
#include "GameObject.h"


class ApplicationManager
{
private:
    Window m_window;
    Draw m_draw;
    bool m_running;

    bool m_backCull;
    unsigned int m_meshMode;

    std::vector<GameObject> m_gameObjects;

    math::mat4 m_viewMat;
    math::mat4 m_projectionMat;

    Camera m_camera;

    std::filesystem::path m_path;

    float m_moveVar = 0.0f;
    float m_incrementMoveVar = 2.0f;

    void MovementInput();
    void MouseInput();

public:
    ApplicationManager(std::filesystem::path&& path);

    void Setup();

    void HandleInputs();
    void Update();
    void Render();

    bool isRunning() const;
};
