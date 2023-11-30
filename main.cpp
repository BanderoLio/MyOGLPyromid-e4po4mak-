#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//
//#define VEC3LOWERLEFTFAR -0.5f, -0.5f, -0.5f
//#define VEC3LOWERLEFTNEAR -0.5f, -0.5f, 0.5f
//#define VEC3LOWERRIGHTFAR 0.5f, -0.5f, -0.5f
//#define VEC3LOWERRIGHTNEAR 0.5f, -0.5f, 0.5f
//#define VEC3UPPERCENTER 0.0f, 0.5f, 0.0f


const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;     //Устанавливаем позицию переменной с координатами в 0

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
)";


void shaderCompileCheck(GLuint shader, GLint &success, GLchar infoLog[]) // Проверка ошибок компиляции шейдера
{
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        printf("%s", infoLog);
        glfwTerminate();

    }
}

//void processInput(GLFWwindow* window) {
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}

int main() {
    GLint success;
    GLchar infoLog[512];
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800, 600, "Pyramid", nullptr, nullptr);
    if (window == nullptr) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
   // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        return -1;
    }

   // glViewport(0, 0, 800, 600);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    shaderCompileCheck(vertexShader, success, infoLog);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    shaderCompileCheck(fragmentShader, success, infoLog);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    shaderCompileCheck(shaderProgram, success, infoLog);  // Проверка ошибок связки шейдерной программы

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    GLfloat vertices[] = {                      //Массив точек, соединяющихся в треугольники

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,

         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,    //Основание пирамиды


        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.0f, 0.5f, 0.0f,


        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        0.0f, 0.5f, 0.0f,


        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        0.0f, 0.5f, 0.0f,


        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.0f, 0.5f, 0.0f

    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
       // processInput(window);
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        
        GLfloat time = glfwGetTime();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, time, glm::vec3(0.0f, 1.0f, 0.0f));
        
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f,
                                      0.1f, 100.0f);
        
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE,
                           value_ptr(model));
        
        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1,
                           GL_FALSE, value_ptr(view));
        
        GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,
                           value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    return 0;
}