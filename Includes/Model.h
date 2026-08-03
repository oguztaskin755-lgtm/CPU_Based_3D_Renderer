#pragma once

#include <filesystem>

#include "Mesh.h"

bool CreateMeshOBJ(Mesh& mesh, const std::filesystem::path& filePath);
void CreateTriangleIndices(std::vector<unsigned int>& currentIndices, std::vector<unsigned int>& currentUVCoords, std::vector<unsigned int>& currentNormals,
    std::vector<unsigned int>& dummyIndices, std::vector<unsigned int>& dummyUVCoords, std::vector<unsigned int>& dummyNormals,
    unsigned int count);
