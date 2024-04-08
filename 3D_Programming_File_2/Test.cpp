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
//// Vertex Shaders
//const char* vertexShaderSource = R"(
//    #version 330 core
//    layout (location = 0) in vec3 aPos;
//    uniform mat4 model;
//    uniform mat4 view;
//    uniform mat4 projection;
//    void main() {
//        gl_Position = projection * view * model * vec4(aPos, 1.0);
//    }
//)";
//
//// Fragment Shaders
//const char* fragmentShaderSource = R"(
//    #version 330 core
//    out vec4 FragColor;
//    uniform vec4 objectColor;
//    void main() {
//        FragColor = objectColor;
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
//// Generate Hill Vertices
//std::vector<GLfloat> generateHillVertices(int width, int length, float step)
//{
//    std::vector<GLfloat> vertices;
//    for (float z = -length / 2; z <= length / 2; z += step)
//    {
//        for (float x = -width / 2; x <= width / 2; x += step)
//        {
//            float y = hillFunction(x, z);
//            vertices.push_back(x);
//            vertices.push_back(y);
//            vertices.push_back(z);
//        }
//    }
//    return vertices;
//}
//
//// Generate Hill Indices
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
//    // Generate Hill Vertices and Indices
//    std::vector<GLfloat> hillVertices = generateHillVertices(10, 10, 0.1f);
//    std::vector<GLuint> hillIndices = generateHillIndices(10, 10, 0.1f);
//
//    // Hill Vertex Buffer
//    GLuint hillVBO, hillVAO, hillEBO;
//    glGenVertexArrays(1, &hillVAO);
//    glGenBuffers(1, &hillVBO);
//    glGenBuffers(1, &hillEBO);
//    glBindVertexArray(hillVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, hillVBO);
//    glBufferData(GL_ARRAY_BUFFER, hillVertices.size() * sizeof(GLfloat), hillVertices.data(), GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hillEBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, hillIndices.size() * sizeof(GLuint), hillIndices.data(), GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    // Sphere Parameters
//    const float sphereRadius = 0.5f;
//    const int sectorCount = 36;
//    const int stackCount = 18;
//
//    // Generate Sphere Vertices
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
//    // Sphere Vertex Buffer
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
//    // Rendering Loop
//    while (!glfwWindowShouldClose(window))
//    {
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        glUseProgram(shaderProgram);
//
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
//        // Draw Hill
//        glBindVertexArray(hillVAO);
//        glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f); // RGBA green
//        glm::mat4 model = glm::mat4(1.0f);
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//        glDrawElements(GL_TRIANGLES, hillIndices.size(), GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0);
//
//        // Draw Spheres atop the hill
//        glm::vec3 spherePositions[] = {
//            glm::vec3(0.0f, hillFunction(-2.0f, -2.0f) + sphereRadius, -3.0f),
//            glm::vec3(0.0f, hillFunction(0.0f, 0.0f) + sphereRadius, 0.0f),
//            glm::vec3(0.0f, hillFunction(2.0f, 2.0f) + sphereRadius, 3.0f)
//        };
//
//        glBindVertexArray(sphereVAO);
//        for (const auto& position : spherePositions)
//        {
//            model = glm::mat4(1.0f);
//            model = glm::translate(model, position);
//            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
//            glUniform4f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 0.5f, 0.2f, 1.0f); // Orange color
//            glDrawArrays(GL_TRIANGLE_FAN, 0, sphereVertices.size() / 3);
//        }
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
//}
//
//int main()
//{
//    GLFWwindow* window = initWindow();
//    renderLoop(window);
//    glfwTerminate();
//    return 0;
//}
