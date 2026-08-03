#pragma once

#include <vector>
#include <cstdint>

#include "../Math/Includes/Transform.h"
#include "Window.h"
#include "Texture.h"

class Draw
{
private:
    //Frame buffers and depth buffers size may change on runtime
    std::vector<uint32_t> m_frameBuffer;
    std::vector<float> m_depthBuffer;

    SDL_Texture* m_texture;
    const Window& m_window;

    void HorizontalBresenham(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);
    void VerticalBresenham(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);

    float CalculatePixelDepth(float depth0, float depth1, float depth2, const math::vec2& point, const math::vec3& baryCentric);

public:
    Draw(const Window& window);
    ~Draw();

    void InitRenderer();

    void RenderFrameBuffer();

    void ClearFrameBuffer(uint32_t color);
    void ClearDepthBuffer();

    void DrawPixel(int32_t posX, int32_t posY, uint32_t color);
    void DrawRectangle(int32_t posX, int32_t posY, int width, int height, uint32_t color);
    void DrawTriangleWireframe(const math::vec2& pos1, const math::vec2& pos2, const math::vec2& pos3, uint32_t color);
    void DrawTriangleWireframe(const math::vec4& pos1, const math::vec4& pos2, const math::vec4& pos3, uint32_t color);

    void DrawLineDDA(const math::vec2& startPos, const math::vec2& endPos, uint32_t color);
    void DrawLineBresenham(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);


    //Pinedas algorithm
    void FillTriangleColor(const math::vec4& ver0, const math::vec4& ver1, const math::vec4& ver2, uint32_t color);
    void FillTriangleTexture(const math::vec4& ver0, const math::vec2& uv0, const math::vec4& ver1, const math::vec2& uv1, const math::vec4& ver2, const math::vec2& uv2, const Texture& texture);

    const Window& getWindow() const;
};
