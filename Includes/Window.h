#pragma once

#include <filesystem>
#include <string>

#include "../Dependencies/SDL/include/SDL3/SDL.h"


class Window
{
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_WindowFlags m_windowFlags;

    const char* m_name;
    const std::string m_iconPath;

    int m_width;
    int m_height;

public:
    Window(SDL_WindowFlags flags, const char* name, const std::filesystem::path& path, int width, int height);
    Window(SDL_WindowFlags flags, const char* name, const std::filesystem::path& path);
    ~Window();

    bool InitializeWindow(const SDL_WindowFlags& flags);
    void ChangeSize(int width, int height);

    SDL_Renderer* getRenderer() const;
    SDL_WindowFlags getWindowFlags() const;
    SDL_Window* getSDLWindow() const;

    constexpr int getWidth() const { return m_width; }
    constexpr int getHeight() const { return m_height; }
};
