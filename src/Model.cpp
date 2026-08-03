#include "Model.h"

#include <fstream>
#include <iostream>
#include <array>
#include <charconv>
#include <string>
#include <string_view>

//Since this is a simple project and this only gets called at the start vector reallocation shouldnt be a huge problem
//HOWEVER IF IT STARTS TO BECOME A PROBLEM RESERVE VECTORS before by first counting the vertexAmount, indexAmount etc..
bool CreateMeshOBJ(Mesh& mesh, const std::filesystem::path& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cerr << "Cant open the file: " << filePath << "\n";
        return false;
    }


    std::vector<math::vec3> vertices;
    std::vector<unsigned int> indices;


    std::vector<unsigned int> currentIndices;
    currentIndices.reserve(3);

    std::vector<math::vec3> vertexNormals;
    std::vector<unsigned int> normalIndices;


    std::vector<unsigned int> currentNormalIndices;
    currentNormalIndices.reserve(3);

    std::vector<math::vec2> vertexTexCoords;
    std::vector<unsigned int> textureIndices;


    std::vector<unsigned int> currentTextureIndices;
    currentTextureIndices.reserve(3);


    //These dummy vectors are being used to improve the speed if we are working with face that has more than 3 vertices
    //This will avoid constant heap allocation/deallocation
    std::vector<unsigned int> dummyIndex;
    dummyIndex.reserve(4);

    std::vector<unsigned int> dummyNormal;
    dummyNormal.reserve(4);

    std::vector<unsigned int> dummyTexture;
    dummyTexture.reserve(4);


    std::string line;

    while (std::getline(file, line))
    {
        //Check if its vertex information
        if (line.rfind("v ", 0) != std::string::npos)
        {
            std::string_view stringView(line);
            stringView.remove_prefix(2);

            std::array<float, 3> vertexPos{};

            for (size_t i = 0; i < 2; i++)
            {
                int pos = stringView.find(' ');

                std::from_chars(stringView.data(), stringView.data() + pos, vertexPos[i]);
                stringView = stringView.substr(pos + 1);
            }

            std::from_chars(stringView.data(), stringView.data() + stringView.size(), vertexPos[2]);


            vertices.emplace_back(vertexPos[0], vertexPos[1], vertexPos[2]);
        }

        else if (line.rfind("vn ", 0) != std::string::npos)
        {
            std::string_view stringView(line);
            stringView.remove_prefix(3);

            std::array<float, 3> vertexNormalPos{};

            for (size_t i = 0; i < 2; i++)
            {
                int pos = stringView.find(' ');

                std::from_chars(stringView.data(), stringView.data() + pos, vertexNormalPos[i]);
                stringView = stringView.substr(pos + 1);
            }

            std::from_chars(stringView.data(), stringView.data() + stringView.size(), vertexNormalPos[2]);

            vertexNormals.emplace_back(vertexNormalPos[0], vertexNormalPos[1], vertexNormalPos[2]);
        }


        else if (line.rfind("vt ", 0) != std::string::npos)
        {
            std::string_view stringView(line);
            stringView.remove_prefix(3);

            std::array<float, 2> textureCoord{};

            int pos = stringView.find(' ');

            std::from_chars(stringView.data(), stringView.data() + pos, textureCoord[0]);
            stringView = stringView.substr(pos + 1);
            std::from_chars(stringView.data(), stringView.data() + stringView.size(), textureCoord[1]);

            vertexTexCoords.emplace_back(textureCoord[0], textureCoord[1]);
        }


        //Check if its face information
        else if (line.rfind("f ", 0) != std::string::npos)
        {
            //Check if its a triangular or a quad mesh
            unsigned int indexPerFace = 0;

            currentIndices.clear();
            currentNormalIndices.clear();
            currentTextureIndices.clear();

            std::string_view stringView(line);

            while (stringView.find(' ') != std::string::npos)
            {
                //Indices in obj files start from 1 not 0 so make sure to -1 from the value
                unsigned int value = 0;
                int pos = 0;

                stringView = stringView.substr(stringView.find(' ') + 1);
                pos = stringView.find('/');
                std::from_chars(stringView.data(), stringView.data() + pos, value);
                currentIndices.push_back(value - 1);

                stringView = stringView.substr(stringView.find('/') + 1);
                pos = stringView.find('/');
                std::from_chars(stringView.data(), stringView.data() + pos, value);
                currentTextureIndices.push_back(value - 1);

                stringView = stringView.substr(stringView.find('/') + 1);


                pos = stringView.find(' ');

                if (pos == std::string::npos)
                {
                    pos = stringView.size();
                }

                std::from_chars(stringView.data(), stringView.data() + pos, value);
                currentNormalIndices.push_back(value - 1);

                indexPerFace++;
            }


            //If its a quad or with multiple vertices per face change indices so that we get multiple triangles
            if (indexPerFace >= 4)
            {
                CreateTriangleIndices(currentIndices, currentTextureIndices, currentNormalIndices, dummyIndex, dummyTexture, dummyNormal, indexPerFace);
            }

            for (size_t i = 0; i < currentIndices.size(); i++)
            {
                indices.push_back(currentIndices[i]);
                textureIndices.push_back(currentTextureIndices[i]);
                normalIndices.push_back(currentNormalIndices[i]);
            }
        }
    }

    mesh = Mesh(std::move(vertices), std::move(indices), std::move(vertexNormals), std::move(normalIndices), std::move(vertexTexCoords), std::move(textureIndices));

    return true;
}

void CreateTriangleIndices(std::vector<unsigned int>& currentIndices, std::vector<unsigned int>& currentUVCoords, std::vector<unsigned int>& currentNormals,
std::vector<unsigned int>& dummyIndices, std::vector<unsigned int>& dummyUVCoords, std::vector<unsigned int>& dummyNormals,
unsigned int count)
{
    //Use the dummy vectors to avoid heap allocation/deallocation everytime this function is called
    dummyIndices.reserve((count - 2) * 3);
    dummyUVCoords.reserve((count - 2) * 3);
    dummyNormals.reserve((count - 2) * 3);

    //Reserve these so when we do swap the dummy vectors can have their original capacity
    currentIndices.reserve((count - 2) * 3);
    currentUVCoords.reserve((count - 2) * 3);
    currentNormals.reserve((count - 2) * 3);

    dummyIndices.clear();
    dummyUVCoords.clear();
    dummyNormals.clear();



    for (size_t i = 0; i < count - 2; i++)
    {
        dummyIndices.push_back(currentIndices[0]);
        dummyIndices.push_back(currentIndices[i + 1]);
        dummyIndices.push_back(currentIndices[i + 2]);

        dummyUVCoords.push_back(currentUVCoords[0]);
        dummyUVCoords.push_back(currentUVCoords[i + 1]);
        dummyUVCoords.push_back(currentUVCoords[i + 2]);

        dummyNormals.push_back(currentNormals[0]);
        dummyNormals.push_back(currentNormals[i + 1]);
        dummyNormals.push_back(currentNormals[i + 2]);
    }


    //Use swap instead of move so that dummy indices still have capacity since move can reset their capacity to 0
    std::swap(currentIndices, dummyIndices);
    std::swap(currentUVCoords, dummyUVCoords);
    std::swap(currentNormals, dummyNormals);
}
