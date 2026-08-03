#include "DeltaTime.h"
#include "../Dependencies/SDL/include/SDL3/SDL.h"



void UpdateDeltaTime()
{
    static uint64_t oldTime = SDL_GetTicksNS();
    uint64_t currentTime = SDL_GetTicksNS();

    deltaTime =  static_cast<float> (currentTime - oldTime) / static_cast<float> (1e9);
    oldTime = currentTime;
}
