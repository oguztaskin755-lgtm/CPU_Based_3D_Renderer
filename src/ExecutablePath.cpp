#include "ExecutablePath.h"

#include <iostream>
#include <vector>
#include <utility>

#if defined(_WIN32)
    #include <libloaderapi.h>
#endif

#if defined(__linux__)
    #include <unistd.h>
    #include <limits.h>
#endif

#if defined(__APPLE__)
    #include <mach-o/dyld.h>
#endif

bool GetExecutablePathDirectory(std::filesystem::path& path)
{
    //Check operating system since each operating system has its own way of getting executable location
#if defined(_WIN32)

    char data[MAX_PATH];
    DWORD returnModule = GetModuleFileName(NULL, data, MAX_PATH);

    //If it cant find the exe
    if (returnModule == 0 || data[0] == '\0')
    {
        std::cerr << "Cant find the Windows executable path \n";
        return false;
    }

    std::filesystem::path exeDirectory = data;
    exeDirectory = exeDirectory.parent_path();
    path = std::move(exeDirectory);

    return true;

#endif

#if defined(__linux__)

    char data[PATH_MAX];
    //Readlink doesnt null terminate so make sure the last element of the data is not written
    ssize_t size = readlink("/proc/self/exe", data, sizeof(data) - 1);

    //If it cant find the exe
    if (size == -1)
    {
        std::cerr << "Cant find the Linux executable path \n";
        return false;
    }

    //Add a null terminator at the end
    data[size] = '\0';

    std::filesystem::path exeDirectory = data;
    exeDirectory = exeDirectory.parent_path();
    path = std::move(exeDirectory);

    return true;

#endif

#if defined(__APPLE__)

    //Mac os gives us the size of the path if it fails, making it much better than windows and linux

    std::vector<char> data(260);
    uint32_t size = 260;

    int enoughSize = _NSGetExecutablePath(data.data(), &size);

    //Check if the size is not enough
    if (enoughSize == -1)
    {
        data.resize(size);
        _NSGetExecutablePath(data.data(), &size);
    }

    std::filesystem::path exeDirectory(data.begin(), data.end());
    exeDirectory = exeDirectory.parent_path();
    path = std::move(exeDirectory);

    return true;

#endif

    std::cerr << "Cant find or unsupported operating system \n";
    return false;
}

