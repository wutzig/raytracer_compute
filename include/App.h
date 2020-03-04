#pragma once
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

typedef unsigned int uint;
const uint MAX_SPHERES = 500;
const uint MAX_CYLINDERS = 500;
const uint WIDTH = 1024;
const uint HEIGHT = 768;

struct UniformLocations{
    std::vector<uint> spherePositions = std::vector<uint>();
    std::vector<uint> sphereColors = std::vector<uint>();
    std::vector<uint> sphereRadius2 = std::vector<uint>();
    std::vector<uint> sphereInvRadius = std::vector<uint>();

    std::vector<uint> cylinderPositions = std::vector<uint>();
    std::vector<uint> cylinderAxes = std::vector<uint>();
    std::vector<uint> cylinderColors = std::vector<uint>();
    std::vector<uint> cylinderRadius = std::vector<uint>();
    std::vector<uint> cylinderLengths = std::vector<uint>();

    std::vector<uint> squarePositions = std::vector<uint>();
    std::vector<uint> squareColors = std::vector<uint>();
    std::vector<uint> squareNormals = std::vector<uint>();
    std::vector<uint> squareUDirections = std::vector<uint>();
    std::vector<uint> squareVDirections = std::vector<uint>();
    std::vector<uint> squareLengths = std::vector<uint>();

    uint cameraDirection, cameraUp, cameraRight, cameraPosition, lightPosition, time,
        numCylinders, numSpheres, texture;
};
class ShaderProgram {
    public:
        ShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath, const char* computeShaderPath);
        uint vertexShaderID;
        uint fragmentShaderID;
        uint shaderProgramID;
        uint computeShaderID;
        void DeleteProgram();
        UniformLocations locations;
    protected:
        uint LoadShader(const char* source, uint type);
        void LoadUniforms();
};
class Renderer{
    public:
        Renderer(uint shaderProgramID);
        void DrawScence(uint shaderProgramID);
        void UseComputeShader();
        void DeleteBuffers();
        void UpdateUniformLocations(UniformLocations uniformLocations);
        void AddSphere(glm::vec3 position, glm::vec3 color, float radius);
        void AddSquare(glm::vec3 position, glm::vec3 normal, glm::vec3 uDirection, glm::vec3 vDirection, glm::vec3 color);
        void AddCylinder(glm::vec3 position, glm::vec3 axis, glm::vec3 color, float radius, float length);
        void AddSegment(int orientation, float direction);
        void CreateObjects();
        void loadBMP(const char* imagepath);
        float delta_time = 0.0f;
        bool grow = true;
    private:
        uint vaoID, vertexBufferID, uvBufferID, elementBufferID, textureID, frameBufferID, computeProgramID,
                     numSpheres = 0, numCylinders = 0;
        glm::vec3 quatRot(glm::vec3 point, glm::vec3 axis, float angle, glm::vec3 origin = glm::vec3(0.0, 0.0, 0.0));
        UniformLocations locations;
        glm::vec3 cameraDirection;
        glm::vec3 cameraRight;
        glm::vec3 cameraUp;
        glm::vec3 cameraPosition;

        std::vector<glm::vec3> cylinder_positions = std::vector<glm::vec3>();
        std::vector<glm::vec3> cylinder_axes = std::vector<glm::vec3>();
        std::vector<glm::vec3> cylinder_colors = std::vector<glm::vec3>();
        std::vector<float> cylinder_radius = std::vector<float>();
        std::vector<float> cylinder_lengths= std::vector<float>();

        std::vector<glm::vec3> sphere_positions = std::vector<glm::vec3>();
        std::vector<float> sphere_radius2 = std::vector<float>();
        std::vector<float> sphere_inv_radius = std::vector<float>();
        std::vector<glm::vec3> sphere_colors = std::vector<glm::vec3>();

        std::vector<glm::vec3> square_positions = std::vector<glm::vec3>();
        float square_length = 1800.0f;
        std::vector<glm::vec3> square_uDirections = std::vector<glm::vec3>();
        std::vector<glm::vec3> square_vDirections = std::vector<glm::vec3>();
        std::vector<glm::vec3> square_normals = std::vector<glm::vec3>();
        std::vector<glm::vec3> square_colors = std::vector<glm::vec3>();

        glm::vec3 lightPosition;
};