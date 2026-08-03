#include "Draw.h"

#include <cmath>

#include "Texture.h"
#include "Geometry.h"


Draw::Draw(const Window& window)
    : m_texture(nullptr), m_window(window)
{

}

Draw::~Draw()
{
    SDL_DestroyTexture(m_texture);
}

//Make sure this is called after SDL is initialized
void Draw::InitRenderer()
{
    //Delete old texture if we are reinitializing because of window size change
    if (m_texture != nullptr)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }

    //Frame buffers size may change on runtime
    m_frameBuffer.resize(m_window.getWidth() * m_window.getHeight());
    m_depthBuffer.resize(m_window.getWidth() * m_window.getHeight());

    m_texture = SDL_CreateTexture(m_window.getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, m_window.getWidth(), m_window.getHeight());
}


void Draw::ClearFrameBuffer(uint32_t color)
{
    std::fill(m_frameBuffer.begin(), m_frameBuffer.end(), color);
}

void Draw::ClearDepthBuffer()
{
    //For the depth buffer we are using the 1 / depth since depth isnt linear transformation which causes problems for barycentric
    //Since its 1 / depth, if the object is farther away its closer to 0 while if the object is closer if its bigger
    std::fill(m_depthBuffer.begin(), m_depthBuffer.end(), 0.0f);
}


void Draw::RenderFrameBuffer()
{
    SDL_UpdateTexture(m_texture, NULL, m_frameBuffer.data(), m_window.getWidth() * sizeof(uint32_t));
    SDL_RenderTexture(m_window.getRenderer(), m_texture, NULL, NULL);
}

float Draw::CalculatePixelDepth(float depth0, float depth1, float depth2, const math::vec2& point, const math::vec3& baryCentric)
{
    //Normally depth is not a linear transformation but 1/depth is linear
    float interpolatedInverseW = ( 1.0f / depth0 * baryCentric.x) + ( 1.0f / depth1 * baryCentric.y) + ( 1.0f / depth2 * baryCentric.z);
    return interpolatedInverseW;
}


void Draw::DrawPixel(int32_t posX, int32_t posY, uint32_t color)
{
    if (posX >= 0 && posY >= 0 && posX < m_window.getWidth() && posY < m_window.getHeight())
    {
        m_frameBuffer[posY * m_window.getWidth() + posX] = color;
    }
}


void Draw::DrawRectangle(int32_t posX, int32_t posY, int width, int height, uint32_t color)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            DrawPixel(j + posX, i + posY, color);
        }
    }
}



//DDA line draw algorithm
void Draw::DrawLineDDA(const math::vec2& startPos, const math::vec2& endPos, uint32_t color)
{
    const auto deltaX = static_cast<int> (endPos.x - startPos.x);
    const auto deltaY = static_cast<int> (endPos.y - startPos.y);

    int stepAmount;

    if (abs(deltaX) > abs(deltaY))
    {
        stepAmount = abs(deltaX);
    }
    else
    {
        stepAmount = abs(deltaY);
    }


    const float incX = (float)deltaX / stepAmount;
    const float incY = (float)deltaY / stepAmount;

    float lineX = startPos.x;
    float lineY = startPos.y;

    for (size_t i = 0; i < stepAmount; i++)
    {
        int roundedLineX = static_cast<int> (std::round(lineX));
        int roundLineY = static_cast<int> (std::round(lineY));

        DrawPixel(roundedLineX, roundLineY, color);

        lineX += incX;
        lineY += incY;
    }
}


//Bresenham line drawing algorithm
void Draw::DrawLineBresenham(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color)
{
    if (abs(x1 - x0) > abs(y1 - y0))
    {
        if (x0 > x1)
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        HorizontalBresenham(x0, y0, x1, y1, color);
    }
    else
    {
        if (y0 > y1)
        {
            std::swap(y0, y1);
            std::swap(x0, x1);
        }
        VerticalBresenham(x0, y0, x1, y1, color);
    }

}



void Draw::HorizontalBresenham(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color)
{
    const int deltaX = x1 - x0;
    const int deltaY = y1 - y0;

    // x << 1 is same as 2 * x
    //Usually compiler optimizes it but still put it here
    const int slope = abs(deltaY) << 1;
    int slopeError = slope - deltaX;

    int currentY = y0;

    int incrementY = deltaY > 0 ? 1 : -1;


    for (int i = x0; i <= x1; i++)
    {
        DrawPixel(i, currentY, color);


        if (slopeError >= 0)
        {
            currentY += incrementY;
            slopeError -= deltaX << 1;
        }
        slopeError += slope;
    }
}

void Draw::VerticalBresenham(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color)
{
    const int deltaX = x1 - x0;
    const int deltaY = y1 - y0;


    // x << 1 is same as 2 * x
    //Usually compiler optimizes it but still put it here
    const int slope = abs(deltaX) << 1;
    int slopeError = slope - deltaY;

    int currentX = x0;

    int incrementX = deltaX > 0 ? 1 : -1;


    for (int i = y0; i <= y1; i++)
    {
        DrawPixel(currentX, i, color);

        if (slopeError >= 0)
        {
            currentX += incrementX;
            slopeError -= deltaY << 1;
        }
        slopeError += slope;
    }
}

void Draw::DrawTriangleWireframe(const math::vec2& pos1, const math::vec2& pos2, const math::vec2& pos3, uint32_t color)
{
    DrawLineBresenham(pos1.x, pos1.y, pos2.x, pos2.y, color);
    DrawLineBresenham(pos2.x, pos2.y, pos3.x, pos3.y, color);
    DrawLineBresenham(pos3.x, pos3.y, pos1.x, pos1.y, color);
}

void Draw::DrawTriangleWireframe(const math::vec4& pos1, const math::vec4& pos2, const math::vec4& pos3, uint32_t color)
{
    DrawLineBresenham(pos1.x, pos1.y, pos2.x, pos2.y, color);
    DrawLineBresenham(pos2.x, pos2.y, pos3.x, pos3.y, color);
    DrawLineBresenham(pos3.x, pos3.y, pos1.x, pos1.y, color);
}



void Draw::FillTriangleColor(const math::vec4& ver0, const math::vec4& ver1, const math::vec4& ver2, uint32_t color)
{
    //Get the bounding box
    const int minX = std::floor(std::min(std::min(ver0.x, ver1.x), ver2.x));
    const int maxX = std::ceil(std::max(std::max(ver0.x, ver1.x), ver2.x));

    const int minY = std::floor(std::min(std::min(ver0.y, ver1.y), ver2.y));
    const int maxY = std::ceil(std::max(std::max(ver0.y, ver1.y), ver2.y));

    const math::vec2 vertex0 = math::toVec2(ver0);
    const math::vec2 vertex1 = math::toVec2(ver1);
    const math::vec2 vertex2 = math::toVec2(ver2);

    //We are using top left rasterization rule
    //TODO: replace the edge functions and bias with fixed points instead of floats because of floating point precision error, currently this causes a tiny gap in the top of the screen
    const float bias0 = isTopLeftEdge(vertex1, vertex0) ? 0.0f : -0.0001f;
    const float bias1 = isTopLeftEdge(vertex2, vertex1) ? 0.0f : -0.0001f;
    const float bias2 = isTopLeftEdge(vertex0, vertex2) ? 0.0f : -0.0001f;


    //Barycentric is linear, instead of calculating it using cross products per pixel we can find how much it changes per row/column and increment it
    const float deltaW0Col = ver1.y - ver0.y;
    const float deltaW1Col = ver2.y - ver1.y;
    const float deltaW2Col = ver0.y - ver2.y;

    const float deltaW0Row = ver0.x - ver1.x;
    const float deltaW1Row = ver1.x - ver2.x;
    const float deltaW2Row = ver2.x - ver0.x;

    const math::vec2 startPoint(minX + 0.5f, minY + 0.5f);

    const float startW0 = math::EdgeFunction(vertex1, vertex0, startPoint) + bias0;
    const float startW1 = math::EdgeFunction(vertex2, vertex1, startPoint) + bias1;
    const float startW2 = math::EdgeFunction(vertex0, vertex2, startPoint) + bias2;

    const float inverseAreaTriangle = 1.0f / math::EdgeFunction(vertex1, vertex0, vertex2);

    for (size_t i = minY; i < maxY; i++)
    {
        bool insideTriangle = false;

        float w0 = startW0 + deltaW0Row * static_cast<float> (i - minY);
        float w1 = startW1 + deltaW1Row * static_cast<float> (i - minY);
        float w2 = startW2 + deltaW2Row * static_cast<float> (i - minY);

        for (size_t j = minX; j < maxX; j++)
        {
            if (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f)
            {
                float alpha = w1 * inverseAreaTriangle;
                float beta = w2 * inverseAreaTriangle;
                float gamma = w0 * inverseAreaTriangle;


                math::vec3 baryCentric(alpha, beta, gamma);

                const float pixelDepth = CalculatePixelDepth(ver0.w, ver1.w, ver2.w, math::vec2(j, i), baryCentric);
                const auto index = static_cast<int> (m_window.getWidth() * i + j);

                if (index >= 0 && index < m_depthBuffer.size())
                {
                    if (pixelDepth > m_depthBuffer[index])
                    {
                        m_depthBuffer[index] = pixelDepth;
                        DrawPixel(j, i, color);
                    }
                }

                insideTriangle = true;
            }

            //If we went from being inside the triangle to outside that means we can skip directly to the next row
            else if (insideTriangle)
            {
                break;
            }

            w0 += deltaW0Col;
            w1 += deltaW1Col;
            w2 += deltaW2Col;
        }
    }
}

void Draw::FillTriangleTexture(const math::vec4& ver0, const math::vec2& uv0, const math::vec4& ver1, const math::vec2& uv1, const math::vec4& ver2, const math::vec2& uv2, const Texture& texture)
{
    //Get the bounding box
    const int minX = std::floor(std::min(std::min(ver0.x, ver1.x), ver2.x));
    const int maxX = std::ceil(std::max(std::max(ver0.x, ver1.x), ver2.x));

    const int minY = std::floor(std::min(std::min(ver0.y, ver1.y), ver2.y));
    const int maxY = std::ceil(std::max(std::max(ver0.y, ver1.y), ver2.y));


    const math::vec2 vertex0 = math::toVec2(ver0);
    const math::vec2 vertex1 = math::toVec2(ver1);
    const math::vec2 vertex2 = math::toVec2(ver2);

    //We are using top left rasterization rule
    //TODO: replace the edge functions and bias with fixed points instead of floats because of floating point precision error, currently this causes a tiny gap in the top of the screen
    const float bias0 = isTopLeftEdge(vertex1, vertex0) ? 0.0f : -0.0001f;
    const float bias1 = isTopLeftEdge(vertex2, vertex1) ? 0.0f : -0.0001f;
    const float bias2 = isTopLeftEdge(vertex0, vertex2) ? 0.0f : -0.0001f;


    //Barycentric is linear, instead of calculating it using cross products per pixel we can find how much it changes per row/column and increment it
    const float deltaW0Col = ver1.y - ver0.y;
    const float deltaW1Col = ver2.y - ver1.y;
    const float deltaW2Col = ver0.y - ver2.y;

    const float deltaW0Row = ver0.x - ver1.x;
    const float deltaW1Row = ver1.x - ver2.x;
    const float deltaW2Row = ver2.x - ver0.x;

    const math::vec2 startPoint(minX + 0.5f, minY + 0.5f);

    const float startW0 = math::EdgeFunction(vertex1, vertex0, startPoint) + bias0;
    const float startW1 = math::EdgeFunction(vertex2, vertex1, startPoint) + bias1;
    const float startW2 = math::EdgeFunction(vertex0, vertex2, startPoint) + bias2;

    const float inverseAreaTriangle = 1.0f / math::EdgeFunction(vertex1, vertex0, vertex2);


    for (size_t i = minY; i < maxY; i++)
    {
        bool insideTriangle = false;

        float w0 = startW0 + deltaW0Row * static_cast<float> (i - minY);
        float w1 = startW1 + deltaW1Row * static_cast<float> (i - minY);
        float w2 = startW2 + deltaW2Row * static_cast<float> (i - minY);

        for (size_t j = minX; j < maxX; j++)
        {
            if (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f)
            {
                const math::vec2 point(j,i);

                const float alpha = w1 * inverseAreaTriangle;
                const float beta = w2 * inverseAreaTriangle;
                const float gamma = w0 * inverseAreaTriangle;


                const math::vec3 baryCentric(alpha, beta, gamma);


                const float pixelDepth = CalculatePixelDepth(ver0.w, ver1.w, ver2.w, point, baryCentric);
                const auto index = static_cast<int> (m_window.getWidth() * i + j);

                if (index >= 0 && index < m_depthBuffer.size())
                {
                    if (pixelDepth > m_depthBuffer[index])
                    {
                        m_depthBuffer[index] = pixelDepth;

                        uint32_t textureColor = TextureColor(ver0.w, uv0, ver1.w, uv1, ver2.w, uv2, point, baryCentric, pixelDepth, texture);
                        DrawPixel(j, i, textureColor);
                    }
                }

                insideTriangle = true;
            }

            //If we went from being inside the triangle to outside that means we can skip directly to the next row
            else if (insideTriangle)
            {
                break;
            }

            w0 += deltaW0Col;
            w1 += deltaW1Col;
            w2 += deltaW2Col;
        }
    }
}

const Window& Draw::getWindow() const
{
    return m_window;
}
