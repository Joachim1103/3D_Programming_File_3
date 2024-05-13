////sources
//// Assistance from ChatGPT
//
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//#include <iostream>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>
//#include <gtc/type_ptr.hpp>
//#include <vector>
//#include <cmath>
//
//#ifndef M_PI
//#define M_PI 3.14159265358979323846
//#endif
//
//// Window Dimensions
//const GLint WIDTH = 1920, HEIGHT = 1080;
//
//// Camera Struct
//struct Camera
//{
//    glm::mat4 view;
//    glm::mat4 projection;
//};
//
//// Camera
//Camera camera;
//
//// Character parameters
//glm::vec3 characterPosition(2.0f, 0.0f, 0.0f);
//const float CHARACTER_SPEED = 0.05f;
//
//// Vertex Shader
//const char* vertexShaderSource = R"(
//    #version 330 core
//    layout (location = 0) in vec3 aPos;
//    layout (location = 1) in vec2 aTexCoord;
//
//    out vec2 TexCoord;
//
//    uniform mat4 model;
//    uniform mat4 view;
//    uniform mat4 projection;
//
//    void main() {
//        gl_Position = projection * view * model * vec4(aPos, 1.0);
//        TexCoord = aTexCoord;
//    }
//)";
//
//// Fragment Shader
//const char* fragmentShaderSource = R"(
//    #version 330 core
//    out vec4 FragColor;
//
//    in vec2 TexCoord;
//
//    uniform sampler2D hillTexture;
//    uniform vec4 objectColor;
//    uniform int useTexture;
//
//    void main() {
//        if (useTexture != 0)
//            FragColor = texture(hillTexture, TexCoord);
//        else
//            FragColor = objectColor;
//    }
//)";
//
//// Hill Parameters
//const float RADIUS = 5.0f;
//const float HEIGHT_SCALE = 2.0f;
//
//// Hill Function
//float hillFunction(float x, float z)
//{
//    float distanceSquared = x * x + z * z;
//    return HEIGHT_SCALE * (1.0f - distanceSquared / (RADIUS * RADIUS));
//}
//
//// Hill Vertices
//std::vector<GLfloat> generateHillVertices(int width, int length, float step) 
//{
//    std::vector<GLfloat> vertices;
//    for (float z = -length / 2; z <= length / 2; z += step) 
//    {
//        for (float x = -width / 2; x <= width / 2; x += step) 
//        {
//            float y = hillFunction(x, z);
//            float s = (x + width / 2) / width;
//            float t = (z + length / 2) / length;
//            vertices.insert(vertices.end(), { x, y, z, s, t });
//        }
//    }
//    return vertices;
//}
//
//// Hill Indices
//std::vector<GLuint> generateHillIndices(int width, int length, float step)
//{
//    std::vector<GLuint> indices;
//    int numVerticesX = width / step + 1;
//    for (int z = 0; z < length / step - 1; ++z)
//    {
//        for (int x = 0; x < width / step - 1; ++x)
//        {
//            int index0 = z * numVerticesX + x;
//            int index1 = index0 + 1;
//            int index2 = index0 + numVerticesX;
//            int index3 = index2 + 1;
//
//            indices.push_back(index0);
//            indices.push_back(index1);
//            indices.push_back(index2);
//
//            indices.push_back(index1);
//            indices.push_back(index3);
//            indices.push_back(index2);
//        }
//    }
//    return indices;
//}
//
//// Load Texture
//GLuint loadTexture(const char* filepath) 
//{
//    GLuint textureID;
//    glGenTextures(1, &textureID);
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    int width, height, nrChannels;
//    stbi_set_flip_vertically_on_load(true);
//    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
//    if (data) 
//    {
//        GLenum format = GL_RGB;
//        if (nrChannels == 1)
//            format = GL_RED;
//        else if (nrChannels == 3)
//            format = GL_RGB;
//        else if (nrChannels == 4)
//            format = GL_RGBA;
//
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//    }
//    else 
//    {
//        std::cerr << "Failed to load texture at " << filepath << std::endl;
//    }
//
//    stbi_image_free(data);
//
//    return textureID;
//}
//
//// Cube Vertices
//std::vector<GLfloat> generateCubeVertices(float size)
//{
//    float halfSize = size / 2.0f;
//    return 
//    {
//        -halfSize, -halfSize, halfSize,
//        halfSize, -halfSize, halfSize,
//        halfSize, halfSize, halfSize,
//        -halfSize, halfSize, halfSize,
//        -halfSize, -halfSize, -halfSize,
//        -halfSize, halfSize, -halfSize,
//        halfSize, halfSize, -halfSize,
//        halfSize, -halfSize, -halfSize,
//        -halfSize, halfSize, -halfSize,
//        -halfSize, halfSize, halfSize,
//        halfSize, halfSize, halfSize,
//        halfSize, halfSize, -halfSize,
//        -halfSize, -halfSize, -halfSize,
//        halfSize, -halfSize, -halfSize,
//        halfSize, -halfSize, halfSize,
//        -halfSize, -halfSize, halfSize,
//        halfSize, -halfSize, -halfSize,
//        halfSize, halfSize, -halfSize,
//        halfSize, halfSize, halfSize,
//        halfSize, -halfSize, halfSize,
//        -halfSize, -halfSize, -halfSize,
//        -halfSize, -halfSize, halfSize,
//        -halfSize, halfSize, halfSize,
//        -halfSize, halfSize, -halfSize
//    };
//}
//
//// Cube Indices
//std::vector<GLuint> generateCubeIndices()
//{
//    return 
//    {
//        0, 1, 2, 0, 2, 3,
//        4, 5, 6, 4, 6, 7,
//        8, 9, 10, 8, 10, 11,
//        12, 13, 14, 12, 14, 15,
//        16, 17, 18, 16, 18, 19,
//        20, 21, 22, 20, 22, 23
//    };
//}
//
//// Parametric Curve Function
//glm::vec2 parametricCurve(float t)
//{
//    float x = 5.0f * t;
//    float y = sinf(t);
//    return glm::vec2(x, y);
//}
//
//// Map Point Using Barycentric Coordinates
//glm::vec3 mapPointToHill(glm::vec2 point)
//{
//    float x = point.x;
//    float z = point.y;
//    float y = hillFunction(x, z);
//
//    return glm::vec3(x, y, z);
//}
//
//// NPC Cube Vertices
//std::vector<GLfloat> generateNPCCubeVertices(float size)
//{
//    float halfSize = size / 2.0f;
//    return 
//    {
//        -halfSize, -halfSize, halfSize,
//        halfSize, -halfSize, halfSize,
//        halfSize, halfSize, halfSize,
//        -halfSize, halfSize, halfSize,
//        -halfSize, -halfSize, -halfSize,
//        -halfSize, halfSize, -halfSize,
//        halfSize, halfSize, -halfSize,
//        halfSize, -halfSize, -halfSize,
//        -halfSize, halfSize, -halfSize,
//        -halfSize, halfSize, halfSize,
//        halfSize, halfSize, halfSize,
//        halfSize, halfSize, -halfSize,
//        -halfSize, -halfSize, -halfSize,
//        halfSize, -halfSize, -halfSize,
//        halfSize, -halfSize, halfSize,
//        -halfSize, -halfSize, halfSize,
//        halfSize, -halfSize, -halfSize,
//        halfSize, halfSize, -halfSize,
//        halfSize, halfSize, halfSize,
//        halfSize, -halfSize, halfSize,
//        -halfSize, -halfSize, -halfSize,
//        -halfSize, -halfSize, halfSize,
//        -halfSize, halfSize, halfSize,
//        -halfSize, halfSize, -halfSize
//    };
//}
//
//// NPC Cube Indices
//std::vector<GLuint> generateNPCCubeIndices()
//{
//    return 
//    {
//        0, 1, 2, 0, 2, 3,
//        4, 5, 6, 4, 6, 7,
//        8, 9, 10, 8, 10, 11,
//        12, 13, 14, 12, 14, 15,
//        16, 17, 18, 16, 18, 19,
//        20, 21, 22, 20, 22, 23
//    };
//}
//
//// NPC Parameters
//glm::vec2 npcParametricPos = parametricCurve(0);
//glm::vec3 npcPosition = mapPointToHill(npcParametricPos);
//const float NPC_SPEED = 0.5f;
//
//// Initialize GLFW
//GLFWwindow* initWindow()
//{
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hill Rendering", nullptr, nullptr);
//    if (window == nullptr)
//    {
//        std::cerr << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        exit(EXIT_FAILURE);
//    }
//
//    glfwMakeContextCurrent(window);
//    gladLoadGL();
//    return window;
//}
//
//// Render Loop
//void renderLoop(GLFWwindow* window)
//{
//    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//    glEnable(GL_DEPTH_TEST);
//
//    camera.projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
//    camera.view = glm::lookAt(glm::vec3(10.0f, 10.0f, 7.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
//    glCompileShader(vertexShader);
//
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
//    glCompileShader(fragmentShader);
//
//    GLuint shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    GLuint hillTexture = loadTexture("texture.png");
//
//    // Hill Vertices and Indices
//    std::vector<GLfloat> hillVertices = generateHillVertices(10, 10, 0.1f);
//    std::vector<GLuint> hillIndices = generateHillIndices(10, 10, 0.1f);
//
//    // VAO, VBO, and EBO for Hill
//    GLuint hillVBO, hillVAO, hillEBO;
//    glGenVertexArrays(1, &hillVAO);
//    glGenBuffers(1, &hillVBO);
//    glGenBuffers(1, &hillEBO);
//    glBindVertexArray(hillVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, hillVBO);
//    glBufferData(GL_ARRAY_BUFFER, hillVertices.size() * sizeof(GLfloat), hillVertices.data(), GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hillEBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, hillIndices.size() * sizeof(GLuint), hillIndices.data(), GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//    glEnableVertexAttribArray(1);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    // Cube Vertices and Indices
//    std::vector<GLfloat> cubeVertices = generateCubeVertices(0.5f);
//    std::vector<GLuint> cubeIndices = generateCubeIndices();
//
//    // VAO, VBO, and EBO for Character
//    GLuint cubeVBO, cubeVAO, cubeEBO;
//    glGenVertexArrays(1, &cubeVAO);
//    glGenBuffers(1, &cubeVBO);
//    glGenBuffers(1, &cubeEBO);
//    glBindVertexArray(cubeVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
//    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(GLfloat), cubeVertices.data(), GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(GLuint), cubeIndices.data(), GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    // NPC Vertices and Indices
//    std::vector<GLfloat> npcVertices = generateNPCCubeVertices(0.5f);
//    std::vector<GLuint> npcIndices = generateNPCCubeIndices();
//
//    // VAO, VBO, and EBO for NPC
//    GLuint npcVBO, npcVAO, npcEBO;
//    glGenVertexArrays(1, &npcVAO);
//    glBindVertexArray(npcVAO);
//    glGenBuffers(1, &npcVBO);
//    glBindBuffer(GL_ARRAY_BUFFER, npcVBO);
//    glBufferData(GL_ARRAY_BUFFER, npcVertices.size() * sizeof(GLfloat), npcVertices.data(), GL_STATIC_DRAW);
//    glGenBuffers(1, &npcEBO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, npcEBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, npcIndices.size() * sizeof(GLuint), npcIndices.data(), GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
//    glBindVertexArray(0);
//
//    // Sphere Parameters
//    const float sphereRadius = 0.5f;
//    const int sectorCount = 36;
//    const int stackCount = 18;
//
//    // Sphere Vertices
//    std::vector<GLfloat> sphereVertices;
//    for (int i = 0; i <= stackCount; ++i)
//    {
//        float stackAngle = M_PI / 2 - i * M_PI / stackCount;
//        float xy = sphereRadius * cosf(stackAngle);
//        float z = sphereRadius * sinf(stackAngle);
//
//        for (int j = 0; j <= sectorCount; ++j)
//        {
//            float sectorAngle = j * 2 * M_PI / sectorCount;
//            float x = xy * cosf(sectorAngle);
//            float y = xy * sinf(sectorAngle);
//
//            sphereVertices.push_back(x);
//            sphereVertices.push_back(y);
//            sphereVertices.push_back(z);
//        }
//    }
//
//    // VAO and VBO for Spheres
//    GLuint sphereVBO, sphereVAO;
//    glGenVertexArrays(1, &sphereVAO);
//    glGenBuffers(1, &sphereVBO);
//    glBindVertexArray(sphereVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
//    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(GLfloat), sphereVertices.data(), GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//    glBindVertexArray(0);
//
//    // Parametric Curve Vertices
//    const int numPoints = 100;
//    std::vector<GLfloat> curveVertices;
//    for (int i = 0; i < numPoints; ++i)
//    {
//        float t = static_cast<float>(i) / (numPoints - 1);
//        glm::vec2 point = parametricCurve(t);
//        glm::vec3 mappedPoint = mapPointToHill(point);
//        curveVertices.push_back(mappedPoint.x);
//        curveVertices.push_back(mappedPoint.y);
//        curveVertices.push_back(mappedPoint.z);
//    }
//
//    // VAO and VBO for Parametric Curve
//    GLuint curveVBO, curveVAO;
//    glGenVertexArrays(1, &curveVAO);
//    glGenBuffers(1, &curveVBO);
//    glBindVertexArray(curveVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, curveVBO);
//    glBufferData(GL_ARRAY_BUFFER, curveVertices.size() * sizeof(GLfloat), curveVertices.data(), GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//    glBindVertexArray(0);
//
//    // Main Render Loop
//    while (!glfwWindowShouldClose(window))
//    {
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        glUseProgram(shaderProgram);
//
//        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
//        GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
//        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.view));
//        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera.projection));
//
//        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
//        GLint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
//
//        // Load Texture Shader
//        GLuint loadTexture(char const* path);
//        GLuint hillTextureID = loadTexture("texture.png");
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, hillTextureID);
//        glUniform1i(glGetUniformLocation(shaderProgram, "hillTexture"), 0);
//
//        // Draw Hill Surface
//        glBindVertexArray(hillVAO);
//        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
//        glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f); // RGBA green
//        glm::mat4 model = glm::mat4(1.0f);
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//        glDrawElements(GL_TRIANGLES, hillIndices.size(), GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0);
//
//        // Character Movement
//        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
//        {
//            characterPosition.z -= CHARACTER_SPEED;
//        }
//        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
//        {
//            characterPosition.z += CHARACTER_SPEED;
//        }
//        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
//        {
//            characterPosition.x -= CHARACTER_SPEED;
//        }
//        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
//        {
//            characterPosition.x += CHARACTER_SPEED;
//        }
//
//        // Character Position
//        float x = characterPosition.x;
//        float z = characterPosition.z;
//        float y = hillFunction(x, z);
//
//        // Draw Character Cube
//        glBindVertexArray(cubeVAO);
//        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
//        glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); // RGBA red
//        model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y + 0.25f, z));
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//        glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0);
//
//        // Sphere Positions
//        glm::vec3 spherePositions[] = 
//        {
//            glm::vec3(0.0f, hillFunction(-2.0f, -2.0f) + sphereRadius, -3.0f),
//            glm::vec3(0.0f, hillFunction(0.0f, 0.0f) + sphereRadius, 0.0f),
//            glm::vec3(0.0f, hillFunction(2.0f, 2.0f) + sphereRadius, 3.0f)
//        };
//
//        // Draw Spheres
//        glBindVertexArray(sphereVAO);
//        for (const auto& position : spherePositions)
//        {
//            model = glm::mat4(1.0f);
//            model = glm::translate(model, position);
//            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
//            glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
//            glUniform4f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 0.5f, 0.2f, 1.0f); // RGBA Orange
//            glDrawArrays(GL_TRIANGLE_FAN, 0, sphereVertices.size() / 3);
//        }
//        glBindVertexArray(0);
//
//        // Draw Parametric Curve
//        glBindVertexArray(curveVAO);
//        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
//        glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f); // RGBA white
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//        glDrawArrays(GL_LINE_STRIP, 0, curveVertices.size() / 3);
//        glBindVertexArray(0);
//
//        // Update NPC Position
//        float oscillatedT = sinf(glfwGetTime() * NPC_SPEED);
//        float normalizedT = (oscillatedT + 1.0f) / 2.0f;
//        glm::vec2 parametricPoint = parametricCurve(normalizedT);
//        npcPosition.x = parametricPoint.x;
//        npcPosition.z = parametricPoint.y;
//        npcPosition.y = hillFunction(npcPosition.x, npcPosition.z);
//
//        // Draw NPC Cube
//        glBindVertexArray(npcVAO);
//        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
//        glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f); // RGBA blue
//        model = glm::translate(model, npcPosition);
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//        glDrawElements(GL_TRIANGLES, npcIndices.size(), GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // Cleanup
//    glDeleteVertexArrays(1, &hillVAO);
//    glDeleteBuffers(1, &hillVBO);
//    glDeleteBuffers(1, &hillEBO);
//    glDeleteVertexArrays(1, &cubeVAO);
//    glDeleteBuffers(1, &cubeVBO);
//    glDeleteBuffers(1, &cubeEBO);
//    glDeleteVertexArrays(1, &sphereVAO);
//    glDeleteBuffers(1, &sphereVBO);
//    glDeleteVertexArrays(1, &curveVAO);
//    glDeleteBuffers(1, &curveVBO);
//    glDeleteVertexArrays(1, &npcVAO);
//    glDeleteBuffers(1, &npcVBO);
//    glDeleteBuffers(1, &npcEBO);
//}
//
//int main()
//{
//    GLFWwindow* window = initWindow();
//    renderLoop(window);
//    glfwTerminate();
//    return 0;
//}
