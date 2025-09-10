#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;


const GLint WIDTH = 800, HEIGHT = 600;
// VAO é o ponteiro do programa que estou rodando, espaço de memória que representa aquele objeto
// VBO é o ponteiro para os meus dados brutos, são os dados do objeto a ser representado pelo VAO 
// shaderProgram é qual programa estou rodando 
// todo programa pode ser chamado de shader
GLuint VAO, VBO, IBO, shaderProgram;

float toRadians = 3.1415f / 180.0f;

bool direction[] = { false, false }, directionScale[] = {false, false};
float triOffset[] = { 0.0f, 0.0f }, triOffsetMax = 0.7f, triOffsetMin = -0.7f, triIncrement[] = { 0.01f, 0.02f };
float triOffsetScale[] = { 0.2f, 0.2f }, triOffsetScaleMax = 1.2f, triOffsetScaleMin = 0.2f, triOffsetScaleIncrement[] = { 0.01f, 0.02f };
float triCurrentAngle = 0.0f, triAngleIncrement = 1.0f;

// vertex lida com o shader
static const char* vertexShader = "                                                                          \n\
#version 330                                                                                                 \n\
                                                                                                             \n\
    //é compilado apenas uma vez e não muda na execução                                                      \n\
layout(location = 0) in vec3 pos;                                                                            \n\
    //^^^^^^^^^^^^^^ é um argumento                                                                          \n\
                                                                                                             \n\
uniform mat4 model;                                                                                          \n\
 // ^^^^^^^^^^^^ -> chamado model pelos calculos que vamos                                                   \n\
 // utilizar para calcular a posicao do nosso modelo atual.                                                  \n\
                                                                                                             \n\
uniform mat4 projection;                                                                                     \n\
                                                                                                             \n\
out vec4 vColor;                                                                                             \n\
// ^^^^^^ -> usado somente para conversa entre vertex e frag!                                                \n\
                                                                                                             \n\
void main() {                                                                                                \n\
    gl_Position = projection * model * vec4(pos, 1.0);                                                       \n\
  //^^^^^^^^^ -> fica estatico e nao mexe no processo!                                                       \n\
                                                                                                             \n\
    vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);                                                             \n\
    // ^^^^^ -> clamp vai deixar tudo no intervalo especificado                                              \n\
}                                                                                                            \n\
";

// fragment lida somente com cores
static const char* fragmentShader = "                                                                       \n\
#version 330                                                                                                \n\
                                                                                                            \n\
uniform vec4 tricolor;                                                                                      \n\
out vec4 color;                                                                                             \n\
// fala que ele tem uma saida do tipo e qual o nome                                                         \n\
in vec4 vColor;                                                                                             \n\
                                                                                                            \n\
                                                                                                            \n\
                                                                                                            \n\
void main() {                                                                                               \n\
    color = vColor;                                                                                         \n\
  //^^^^^^                                                                                                  \n\
}                                                                                                           \n\
";

void CreateVertices() {
    unsigned int indices[] = {
        0, 1, 2,
        0, 1, 3,
        0, 2, 3,
        1, 2, 3
    };
    // IBO -> Indice Buffer Object

    GLfloat vertices[] = {
        0.0f, 1.0f, 0.0f,      // Vertice 1
        1.0f, -1.0f, 0.0f,       // Vertice 2
        -1.0f, -1.0f, 0.0f,       //Vertice 3
        0.0f, 0.0f, 1.0f       //Vertice 4
    };

    glGenVertexArrays(1, &VAO);
    // Quantos VAOs estou alocando
    glBindVertexArray(VAO);

        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            // ( tipo, tamanho, item, tipo de uso (static -> sem mudanças )

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            // ( Index de inicio, quantos items vou ler, tipo do item, se esta normalizado, quantos vou pular depois desses x e y, placeholder(?)
            glEnableVertexAttribArray(0);  // location
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)widthBuffer/(GLfloat)heightBuffer, 0.1f, 100.0f);
    // a nossa camera angulada 45graus pra baixo, a distancia dos dois, o quao perto e o quao longe da camera vai renderizar

    // desenhamos a tela enquanto não a fechamos
    while (!glfwWindowShouldClose(window)) {
        // cor de fundo da janela
        glClearColor(0.55f, 0.15f, 0.48f, 1.0f);
        //ele que organiza os eventos que vão ser renderizados
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        // ----- desenhando o triangulo
        
        //altera a cor do triangulo
        /*
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	    float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	    float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        GLint uniformColor = glGetUniformLocation(shaderProgram, "tricolor");
            //retorna onde esta o ponteiro para popular, tal a variavel do fragment
        glUniform4f(uniformColor, r, g, b, 1.0f);
        */

        //movimenta o triangulo

        if (!direction[0]) {
            triOffset[0] += triIncrement[0];
        }
        else {
            triOffset[0] -= triIncrement[0];
        }
        
        if (!direction[1]) {
            triOffset[1] += triIncrement[1];
        }
        else {
            triOffset[1] -= triIncrement[1];
        }

        if (triOffset[0] > triOffsetMax || triOffset[0] < triOffsetMin) {
            direction[0] = !direction[0];
        }
        if (triOffset[1] > triOffsetMax || triOffset[1] < triOffsetMin) {
            direction[1] = !direction[1];
        }

        triCurrentAngle += triAngleIncrement;
        if (triCurrentAngle >= 360)
            triCurrentAngle = 0;

        if (!directionScale[0]) {
            triOffsetScale[0] += triIncrement[0];
        }
        else {
            triOffsetScale[0] -= triIncrement[0];
        }

        if (!directionScale[1]) {
            triOffsetScale[1] += triIncrement[1];
        }
        else {
            triOffsetScale[1] -= triIncrement[1];
        }

        if (triOffsetScale[0] > triOffsetScaleMax || triOffsetScale[0] < triOffsetScaleMin) {
            directionScale[0] = !directionScale[0];
        }
        if (triOffsetScale[1] > triOffsetScaleMax || triOffsetScale[1] < triOffsetScaleMin) {
            directionScale[1] = !directionScale[1];
        }

        GLint uniformProjection = glGetUniformLocation(shaderProgram, "projection");


        GLint uniformModel = glGetUniformLocation(shaderProgram, "model");
        //^^^^^^^^^^^^^ <- ENDEREÇO DE POSX E NÃO SEU VALOR
        glm::mat4 model(1.0f);
        //^^^^^-> cria uma matriz 4x4 cheia de 1

        //model = glm::translate(model, glm::vec3(triOffset[0], triOffset[1], 0.f));
        //model = glm::scale(model, glm::vec3(triOffsetScale[0], triOffsetScale[1], 0.0f));

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        model = glm::rotate(model, triCurrentAngle * toRadians, glm::vec3(0.7f, 0.5f, 1.0f));
        
        
        
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        // ^^^^^^^^^^^^^^^ -> (ENDEREÇO, QUANTOS MODELS, SE ESTA TRANSPOSTA, VALOR MAS CONVERTIDO PARA PONTEIRO)
        

            
        // inicia o programa
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
                glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
                // ( qual modelo estamos usando, quantas ligações temos que fazer, qual tipo, nenhuma posicao pra pular )
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        //Matou o programa
        glBindVertexArray(0);


        // chama o VAO, que é onde o triangulo está.
    //    glBindVertexArray(VAO);
    //        glDrawArrays(GL_TRIANGLES, 0, 3); // Triangulo, começando na posição 0, Numero de pontos 3
    //    glBindVertexArray(0);



        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
