#include "Window.h"
#include "../Dependencies/STB Image/stb_image.h"

#include <iostream>
#include <ostream>

Window::Window(SDL_WindowFlags flags, const char* name, const std::filesystem::path& path, int width, int height)
    :m_windowFlags(flags), m_name(name), m_iconPath(path.string()), m_width(width), m_height(height), m_window(nullptr), m_renderer(nullptr)
{

}

Window::Window(SDL_WindowFlags flags, const char* name, const std::filesystem::path& path)
    :m_windowFlags(flags), m_name(name), m_iconPath(path.string()), m_width(0), m_height(0), m_window(nullptr), m_renderer(nullptr)
{

}


Window::~Window()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool Window::InitializeWindow(const SDL_WindowFlags& flags)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Error initializing SDL %s", SDL_GetError());
        return false;
    }

    bool shouldFullscren = false;

    //If width or height is 0 make it fullscreen
    if (m_width == 0 | m_height == 0)
    {
        SDL_DisplayID* displayID = SDL_GetDisplays(NULL);
        const SDL_DisplayMode* displayMode = SDL_GetCurrentDisplayMode(*displayID);

        m_width = displayMode->w;
        m_height = displayMode->h;


        shouldFullscren = true;
    }

    m_window = SDL_CreateWindow(m_name, m_width, m_height, flags);

    if (m_window == nullptr)
    {
        SDL_Log("Error creating window");
        return false;
    }

    if (shouldFullscren)
    {
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
    }

    m_renderer = SDL_CreateRenderer(m_window, NULL);

    if (m_renderer == nullptr)
    {
        SDL_Log("Error creating renderer");
        return false;
    }

    int iconWidth, iconHeight;

    const char* iconChar = m_iconPath.c_str();
    unsigned char* icon = stbi_load(iconChar, &iconWidth, &iconHeight, NULL, 4);

    if (icon == nullptr)
    {
        SDL_Log("Error loading icon image: %s", iconChar);
        return false;
    }

    SDL_Surface* iconSurface = SDL_CreateSurfaceFrom(iconWidth, iconHeight, SDL_PIXELFORMAT_RGBA32, icon, 4 * iconWidth);

    SDL_SetWindowIcon(m_window, iconSurface);

    return true;
}

void Window::ChangeSize(int width, int height)
{
    m_width = width;
    m_height = height;

    SDL_SetWindowSize(m_window, width, height);
}

SDL_Renderer* Window::getRenderer() const
{
    return m_renderer;
}

SDL_WindowFlags Window::getWindowFlags() const
{
    return m_windowFlags;
}

SDL_Window *Window::getSDLWindow() const
{
    return m_window;
}

