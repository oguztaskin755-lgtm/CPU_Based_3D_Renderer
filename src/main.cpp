#include <filesystem>
#include <utility>


#include "../Includes/ApplicationManager.h"
#include "ExecutablePath.h"


int main()
{
    std::filesystem::path path;

    if (!GetExecutablePathDirectory(path))
    {
        return -1;
    }


    ApplicationManager manager(std::move(path));
    manager.Setup();


    while (manager.isRunning())
    {
        manager.HandleInputs();
        manager.Update();
        manager.Render();
    }

    return 0;
}