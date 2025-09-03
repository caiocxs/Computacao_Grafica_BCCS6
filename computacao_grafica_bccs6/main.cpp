#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <time.h>
using namespace std;


const GLint WIDTH = 800, HEIGHT = 600;
// VAO é o ponteiro do programa que estou rodando, espaço de memória que representa aquele objeto
// VBO é o ponteiro para os meus dados brutos, são os dados do objeto a ser representado pelo VAO 
// shaderProgram é qual programa estou rodando 
// todo programa pode ser chamado de shader
GLuint VAO, VBO, shaderProgram;

static const char* vertexShader = "                                 \n\
#version 330                                                        \n\
                                                                    \n\
    //é compilado apenas uma vez e não muda na execução             \n\
layout(location = 0) in vec2 pos;                                   \n\
    //^^^^^^^^^^^^^^ é um argumento                                 \n\
                                                                    \n\
void main() {                                                       \n\
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);                     \n\
  //^^^^^^^^^ -> fica estatico e nao mexe no processo!              \n\
}                                                                   \n\
";

static const char* fragmentShader = "                               \n\
#version 330                                                        \n\
                                                                    \n\
uniform vec4 tricolor;                                              \n\
out vec4 color;                                                     \n\
// fala que ele tem uma saida do tipo e qual o nome                 \n\
                                                                    \n\
void main() {                                                       \n\
    color = vec4(tricolor);                                         \n\
  //^^^^^^                                                          \n\
}                                                                   \n\
";

void CreateVertices() {
    GLfloat vertices[] = {
        0.0f, -1.0f,     // Vertice 1
        1.0f, 1.0f,   // Vertice 2
        -1.0f, 1.0f     //Vertice 3
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

void AddShader(GLuint program, const char* charCode, GLenum type) {
// Aqui que é Compilar os Shaders!
    GLuint _shader = glCreateShader(type);
    // _item para sinalizar que eh um item local e temporario, e não global.

    const GLchar* code[1];
    code[0] = charCode;
    //fica apontado pro charCode, assim tendo um ponteiro pro endereço correto.

    glShaderSource(_shader, 1, code, NULL);
    // ( shader, quantos codigos vai rodar, codigo, tamanho (pode ser null))
    glCompileShader(_shader);
    // compila o shader


    // tratar os erros

    glAttachShader(program, _shader);
    // adiciona shader no programa
}

void AddProgram() {
    shaderProgram = glCreateProgram();

    if (!shaderProgram) {
        printf("Erro ao criar o programa!");
        return;
    }

    AddShader(shaderProgram, vertexShader, GL_VERTEX_SHADER);
    AddShader(shaderProgram, fragmentShader, GL_FRAGMENT_SHADER);

    glLinkProgram(shaderProgram);
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

    // obtendo os tamanhos reais
    int widthBuffer, heightBuffer;
    glfwGetFramebufferSize(window, &widthBuffer, &heightBuffer);

    glfwMakeContextCurrent(window); // tornando a janela como principal

    // cria o glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Erro ao iniciar o GLEW");

        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // cria a janela tal que vamos escrever o programa
    glViewport(0, 0, widthBuffer, heightBuffer);



    CreateVertices();
    AddProgram();
    // desenhamos a tela enquanto não a fechamos
    while (!glfwWindowShouldClose(window)) {
        // cor de fundo da janela
        glClearColor(0.55f, 0.15f, 0.48f, 1.0f);
        //ele que organiza os eventos que vão ser renderizados
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        srand(time(0));
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		    float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		    float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        printf("%d, %d, %d\n", r, g, b);

        // ----- desenhando o triangulo
        //altera a cor do triangulo
        GLint uniformColor = glGetUniformLocation(shaderProgram, "tricolor");
            //retorna onde esta o ponteiro para popular, tal a variavel do fragment
        glUniform4f(uniformColor, r, g, b, 1.0f);

            
        // inicia o programa
        glUseProgram(shaderProgram);
        // chama o VAO, que é onde o triangulo está.
        glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3); // Triangulo, começando na posição 0, Numero de pontos 3
        glBindVertexArray(0);



        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
