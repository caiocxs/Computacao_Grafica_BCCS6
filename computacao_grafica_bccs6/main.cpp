#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shaderProgram;

static const char* vertexShader = "                                 \n\
#version 330                                                        \n\
                                                                    \n\
layout(location = 0) in vec2 pos;                                   \n\
                                                                    \n\
void main() {                                                       \n\
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);                     \n\
}                                                                   \n\
";

static const char* fragmentShader = "                               \n\
#version 330                                                        \n\
                                                                    \n\
uniform in vec4 rgba;                                               \n\
                                                                    \n\
void main() {                                                       \n\
    color = vec4(rgba);                                             \n\
}                                                                   \n\
";

void CreateTriangle() {
    GLfloat vertices[] = {
        0.0f, 1.0f,     // Vertice 1
        -1.0f, -1.0f,   // Vertice 2
        1.0f, -1.0f     //Vertice 3
    };

    glGenVertexArrays(1, &VAO);
    // Quantos VAOs estou alocando
    glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            // ( tipo, tamanho, item, tipo de uso )
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
            // ( Index de inicio, quantos items vou ler, tipo do item, se esta normalizado, quantos vou pular depois desses x e y, placeholder(?)
            glEnableVertexAttribArray(0);  // location
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddTriangle(GLuint program, char* charCode, GLenum type) {
    GLuint _shader = glCreateShader(type);
    // _item para sinalizar que eh um item local e temporario, e nao global.

    const GLchar* code[1];
    code[0] = charCode;
    //fica apontado pro charCode, assim tendo um ponteiro pro endereço correto.

    glShaderSource(_shader, 1, code, NULL);
    // ( shader, quantos codigos vai rodar, codigo, tamanho (pode ser null))
    glCompileShader(_shader);
    //  compila o shader


    //tratar os erros
  
    glAttachShader(program, _shader);
}

int main() {
  
    if (!glfwInit()) {
        printf("GLFW não foi inicializado!");
    
        glfwTerminate();
        return 1;
    }

    // set nas coisas obrigatórias para subir uma janela
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // versão máxima permitida 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // versão mínima
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // somente as funções core
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // qual a precisão de ponto flutuante que vamos usar, precisão da placa


    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "PRIMEIRO", NULL, NULL);

    if (!window) {
        printf("Erro ao criar a janela!");
        glfwTerminate();
        return 1;
    }

    int widthBuffer, heightBuffer;

    glfwGetFramebufferSize(window, &widthBuffer, &heightBuffer);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        printf("Erro ao iniciar o GLEW");

        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, widthBuffer, heightBuffer);
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.4f, 0.1f, 0.2f, 0.9f);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
