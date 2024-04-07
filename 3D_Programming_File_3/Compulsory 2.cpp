//sources
// Assistance from ChatGPT

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>

// Window Dimensions
const GLint WIDTH = 1920, HEIGHT = 1080;

// Camera Struct
struct Camera 
{
    glm::mat4 view;
    glm::mat4 projection;
};

// Camera
Camera camera;

// Vertex Shaders
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)";

// Fragment Shaders
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec4 objectColor;
    void main() {
        FragColor = objectColor; // Use the color passed as a uniform
    }
)";

// Vertex data for Player
GLfloat playerVertices[] = 
{
    -0.5f, -0.5f, 0.5f,
     0.5f, -0.5f, 0.5f,
     0.5f,  0.5f, 0.5f,
    -0.5f,  0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f
};

// Indices for Player
GLuint playerIndices[] = 
{
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    4, 0, 3, 3, 7, 4,
    1, 5, 6, 6, 2, 1,
    3, 2, 6, 6, 7, 3,
    0, 1, 5, 5, 4, 0
};

// Vertex Data for NPC
GLfloat npcVertices[] = 
{
    -0.25f, -0.25f, 0.25f,
    0.25f, -0.25f, 0.25f,
    0.25f, 0.25f, 0.25f,
    -0.25f, 0.25f, 0.25f,
    -0.25f, -0.25f, -0.25f,
    0.25f, -0.25f, -0.25f,
    0.25f, 0.25f, -0.25f,
    -0.25f, 0.25f, -0.25f
};

// Indices for NPC
GLuint npcIndices[] = 
{
    0, 1, 2,
    2, 3, 0,
    4, 5, 6,
    6, 7, 4,
    4, 0, 3,
    3, 7, 4,
    1, 5, 6,
    6, 2, 1,
    3, 2, 6,
    6, 7, 3,
    0, 1, 5,
    5, 4, 0
};

// NPC Position and Movement Speed
bool npcOnPath1 = true;
glm::vec3 npcPosition1 = glm::vec3(-1.5f, -0.2f, 0.0f);
glm::vec3 npcPosition2 = glm::vec3(0.0f, -0.2f, 0.5f);
glm::vec3 npcPosition3 = glm::vec3(0.0f, -0.2f, 0.5f);
glm::vec3 npcPosition4 = glm::vec3(0.0f, -0.2f, -1.0f);
glm::vec3 npcPosition;
float npcSpeed = 1.0f;

// NPC Movement
void updateNPCPosition(float deltaTime) 
{
    glm::vec3& currentStartPos = npcOnPath1 ? npcPosition1 : npcPosition3;
    glm::vec3& currentEndPos = npcOnPath1 ? npcPosition2 : npcPosition4;

    glm::vec3 direction = glm::normalize(currentEndPos - npcPosition);
    npcPosition += direction * npcSpeed * deltaTime;

    if (glm::length(npcPosition - currentEndPos) < 0.05f) 
    {
        std::swap(currentStartPos, currentEndPos);
    }
}

const int SPHERE_SECTORS = 36;
const int SPHERE_STACKS = 18;

std::vector<GLfloat> sphereVertices;
std::vector<GLuint> sphereIndices;

void createSphere(float radius, int sectors, int stacks) 
{
    for (int i = 0; i <= stacks; ++i) 
    {
        float stackAngle = glm::pi<float>() * static_cast<float>(i) / stacks;
        float stackRadius = radius * sin(stackAngle);
        float stackHeight = radius * cos(stackAngle);
        for (int j = 0; j <= sectors; ++j) 
        {
            float sectorAngle = 2 * glm::pi<float>() * static_cast<float>(j) / sectors;
            float x = stackRadius * sin(sectorAngle);
            float z = stackRadius * cos(sectorAngle);
            float y = stackHeight;
            sphereVertices.push_back(x);
            sphereVertices.push_back(y);
            sphereVertices.push_back(z);
        }
    }
    for (int i = 0; i < stacks; ++i) 
    {
        for (int j = 0; j < sectors; ++j) 
        {
            int k0 = i * (sectors + 1) + j;
            int k1 = k0 + 1;
            int k2 = (i + 1) * (sectors + 1) + j + 1;
            int k3 = (i + 1) * (sectors + 1) + j;
            sphereIndices.push_back(k0);
            sphereIndices.push_back(k1);
            sphereIndices.push_back(k2);
            sphereIndices.push_back(k0);
            sphereIndices.push_back(k2);
            sphereIndices.push_back(k3);
        }
    }
}

std::vector<glm::vec3> spherePositions;

// Sphere Positions
void initSpherePositions() 
{
    spherePositions.push_back(glm::vec3(-1.5f, -0.4f, 0.0f));
    spherePositions.push_back(glm::vec3(0.0f, -0.4f, 0.5f));
    spherePositions.push_back(glm::vec3(2.5f, -0.4f, 0.2f));
    spherePositions.push_back(glm::vec3(1.5f, -0.4f, 2.0f));
    spherePositions.push_back(glm::vec3(0.0f, -0.4f, 2.0f));
    spherePositions.push_back(glm::vec3(0.0f, -0.4f, -1.0f));
}

// Player Position and Movement Speed
glm::vec3 playerPosition = glm::vec3(1.0f, -0.4, 2.0f);
float playerSpeed = 0.001f;

bool isNear(glm::vec3 point1, glm::vec3 point2, float distanceThreshold) 
{
    float distanceSquared = glm::length(point1 - point2);
    return distanceSquared <= (distanceThreshold * distanceThreshold);
}

bool cKeyWasPressed = false;
bool isInHouse = false;

// Keyboard Input
void processInput(GLFWwindow* window) 
{
    static bool cKeyWasPressed = false;

    float sprintFactor = 1.0f;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
    {
        sprintFactor = 2.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
    {
        playerPosition.z -= playerSpeed * sprintFactor;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
    {
        playerPosition.z += playerSpeed * sprintFactor;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
    {
        playerPosition.x -= playerSpeed * sprintFactor;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
    {
        playerPosition.x += playerSpeed * sprintFactor;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) 
    {
        for (auto it = spherePositions.begin(); it != spherePositions.end(); ) 
        {
            if (isNear(playerPosition, *it, 0.3f)) 
            {
                it = spherePositions.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    bool cKeyPressed = glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS;
    if (cKeyPressed && !cKeyWasPressed)
    {
        npcOnPath1 = !npcOnPath1;
        npcPosition = npcOnPath1 ? npcPosition1 : npcPosition3;
    }
    cKeyWasPressed = cKeyPressed;

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) 
    {
        glm::vec3 doorPosition = glm::vec3(1.3f, -0.45f, 0.01f);
        if (isNear(playerPosition, doorPosition, 0.3f)) 
        {
            isInHouse = !isInHouse;
            if (isInHouse) 
            {
                playerPosition = glm::vec3(0.0f, -0.4f, 0.0f);

                camera.view = glm::lookAt(
                    glm::vec3(-0.5f, 1.0f, 1.0f),
                    glm::vec3(0.0f, 0.0f, 0.0f), 
                    glm::vec3(0.0f, 1.0f, 0.0f)
                );
            }
            else 
            {
                playerPosition = glm::vec3(1.5f, -0.4f, 0.5f);
                camera.view = glm::lookAt(glm::vec3(1.0f, 0.0f, 3.5f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
        }
    } 
}

// Initialize GLFW
void initWindow() 
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Simple 3D Game", nullptr, nullptr);
    if (window == nullptr) 
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
}

// Render Loop
void renderLoop(GLFWwindow* window) 
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    camera.projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    camera.view = glm::lookAt(glm::vec3(1.0f, 0.0f, 3.5f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex Data for Ground Plane
    GLfloat planeVertices[] = 
    {
        -2.0f, -0.5f, -2.0f,
        2.0f, -0.5f, -2.0f,
        2.0f, -0.5f, 3.0f,
        -2.0f, -0.5f, 3.0f
    };

    // Indices for Ground Plane
    GLuint planeIndices[] = 
    {
        0, 1, 2,
        2, 3, 0
    };

    // Vertex Data for House
    GLfloat houseVertices[] = 
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 1.0f,
        0.5f, -0.5f, 1.0f,
        0.5f, 0.5f, 1.0f,
        -0.5f, 0.5f, 1.0f,
        0.0f, 1.0f, 0.5f,
        -0.1f, -0.495f, 0.0f,
        0.1f, -0.51f, 0.0f,
        0.1f, 0.0f, 0.0f,
        -0.1f, 0.0f, 0.0f
    };

    // Indices for House
    GLuint houseIndices[] = 
    {
        0, 1, 2,
        2, 3, 0,
        4, 5, 6,
        6, 7, 4,
        0, 4, 7,
        7, 3, 0,
        1, 5, 6,
        6, 2, 1,
        0, 1, 5,
        5, 4, 0,
        3, 2, 6,
        6, 7, 3,
        9, 10, 11,
        11, 12, 9
    };

    // Vertex data for Interior
    GLfloat houseInteriorVertices[] = 
    {
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
         0.5f, 0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, 0.5f, -0.5f,
         0.5f, 0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
    };

    // Indices for Interior
    GLuint houseInteriorIndices[] = 
    {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6, 
        4, 6, 7,
        8, 9, 10,
        8, 10, 11
    };

    // VAO, VBO, and EBO for Ground Plane
    GLuint planeVAO, planeVBO, planeEBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glGenBuffers(1, &planeEBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // VAO, VBO, and EBO for House
    GLuint houseVAO, houseVBO, houseEBO;
    glGenVertexArrays(1, &houseVAO);
    glGenBuffers(1, &houseVBO);
    glGenBuffers(1, &houseEBO);
    glBindVertexArray(houseVAO);
    glBindBuffer(GL_ARRAY_BUFFER, houseVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(houseVertices), houseVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, houseEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(houseIndices), houseIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // VAO, VBO, and EBO for Player
    GLuint playerVAO, playerVBO, playerEBO;
    glGenVertexArrays(1, &playerVAO);
    glGenBuffers(1, &playerVBO);
    glGenBuffers(1, &playerEBO);
    glBindVertexArray(playerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, playerVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(playerVertices), playerVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, playerEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(playerIndices), playerIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // VAO, VBO, and EBO for NPC
    GLuint npcVAO, npcVBO, npcEBO;
    glGenVertexArrays(1, &npcVAO);
    glGenBuffers(1, &npcVBO);
    glGenBuffers(1, &npcEBO);
    glBindVertexArray(npcVAO);
    glBindBuffer(GL_ARRAY_BUFFER, npcVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(npcVertices), npcVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, npcEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(npcIndices), npcIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // VAO, VBO, and EBO for Sphere
    GLuint sphereVAO, sphereVBO, sphereEBO;
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);
    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(GLfloat), sphereVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(GLuint), sphereIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // VAO, VBO, and EBO for Interior
    GLuint houseInteriorVAO, houseInteriorVBO, houseInteriorEBO;
    glGenVertexArrays(1, &houseInteriorVAO);
    glGenBuffers(1, &houseInteriorVBO);
    glGenBuffers(1, &houseInteriorEBO);
    glBindVertexArray(houseInteriorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, houseInteriorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(houseInteriorVertices), houseInteriorVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, houseInteriorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(houseInteriorIndices), houseInteriorIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    initSpherePositions();

    // Main Render Loop
    while (!glfwWindowShouldClose(window)) 
    {
        processInput(window);
        float deltaTime = 0.001f;
        updateNPCPosition(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera.projection));

        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");

        if (!isInHouse) 
        {
            // Draw Plane
            glBindVertexArray(planeVAO);
            glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f); // RGBA green
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // Draw House
            glBindVertexArray(houseVAO);
            glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); // RGBA red
            model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // Draw Door
            glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f); // RGBA blue
            model = glm::mat4(1.0f);
            float DoorWidth = 0.3f;
            float DoorHeight = 1.0f;
            model = glm::translate(model, glm::vec3(1.3f, -0.45f + DoorHeight / 2, 0.01f));
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(houseVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(36 * sizeof(GLuint)));

            // Draw Player
            glUniform4f(colorLoc, 0.5f, 0.0f, 0.5f, 1.0f); // RGBA purple
            glBindVertexArray(playerVAO);
            glm::mat4 playerModel = glm::mat4(1.0f);
            playerModel = glm::translate(playerModel, playerPosition);
            playerModel = glm::scale(playerModel, glm::vec3(0.1f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(playerModel));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // Draw NPC
            glUniform4f(colorLoc, 1.0f, 0.5f, 0.0f, 1.0f); // RGBA orange
            glBindVertexArray(npcVAO);
            glm::mat4 npcModel = glm::mat4(1.0f);
            npcModel = glm::translate(npcModel, npcPosition);
            npcModel = glm::scale(npcModel, glm::vec3(0.1f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(npcModel));
            glBindVertexArray(playerVAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // NPC Movement
            if (npcOnPath1)
            {
                npcPosition += glm::normalize(npcPosition2 - npcPosition1) * npcSpeed * deltaTime;
                if (glm::length(npcPosition - npcPosition2) < 0.001f) 
                {
                    npcOnPath1 = false;
                }
            }
            else {
                npcPosition += glm::normalize(npcPosition1 - npcPosition2) * npcSpeed * deltaTime;
                if (glm::length(npcPosition - npcPosition1) < 0.001f) 
                {
                    npcOnPath1 = true;
                }
            }

            // Draw Spheres
            glBindVertexArray(sphereVAO);
            for (const auto& position : spherePositions) 
            {
                glm::mat4 sphereModel = glm::mat4(1.0f);
                sphereModel = glm::translate(sphereModel, position);
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sphereModel));
                glUniform4f(colorLoc, 1.0f, 0.843f, 0.0f, 1.0f); // RGBA gold
                glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphereIndices.size()), GL_UNSIGNED_INT, 0);

            }
        }
        else 
        {
            // Draw Sphere Inside
            glm::vec3 greenSpherePosition = glm::vec3(0.35f, -0.4f, -0.3f);
            glBindVertexArray(sphereVAO);
            glm::mat4 sphereModel = glm::mat4(1.0f);
            sphereModel = glm::translate(sphereModel, greenSpherePosition);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sphereModel));
            glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f); // RGBA green
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphereIndices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // Draw Player Inside
            glBindVertexArray(playerVAO);
            glm::mat4 playerModel = glm::mat4(1.0f);
            playerModel = glm::translate(playerModel, playerPosition);
            playerModel = glm::scale(playerModel, glm::vec3(0.1f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(playerModel));
            glUniform4f(colorLoc, 0.5f, 0.0f, 0.5f, 1.0f); // RGBA purple
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // Draw Interior
            glBindVertexArray(houseInteriorVAO);
            glUniform4f(colorLoc, 0.8f, 0.8f, 0.8f, 1.0f); // RGBA grey
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawElements(GL_TRIANGLES, sizeof(houseInteriorIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Delete Resources
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &planeEBO);
    glDeleteVertexArrays(1, &houseVAO);
    glDeleteBuffers(1, &houseVBO);
    glDeleteBuffers(1, &houseEBO);
    glDeleteVertexArrays(1, &playerVAO);
    glDeleteBuffers(1, &playerVBO);
    glDeleteBuffers(1, &playerEBO);
    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteBuffers(1, &sphereVBO);
    glDeleteBuffers(1, &sphereEBO);
    glDeleteVertexArrays(1, &npcVAO);
    glDeleteBuffers(1, &npcVBO);
    glDeleteBuffers(1, &npcEBO);
    glDeleteVertexArrays(1, &houseInteriorVAO);
    glDeleteBuffers(1, &houseInteriorVBO);
    glDeleteBuffers(1, &houseInteriorEBO);
    glDeleteProgram(shaderProgram);
}

int main()
{
    initWindow();
    createSphere(0.05f, SPHERE_SECTORS, SPHERE_STACKS);
    npcPosition = npcPosition1;
    renderLoop(glfwGetCurrentContext());
    glfwTerminate();
    return 0;
}