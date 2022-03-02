
#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// GLAD es una libreria "loader-generator"; OpenGL requiere este tipo de libreria (de la que existen 
// varias implementaciones dependiendo de la plataforma)
#include "deps/glad.h"

//-- SOKOL 
// Sokol es una libreria simple portable para abrir ventanas
#define SOKOL_GLCORE33 1
#define SOKOL_APP_IMPL 1
#define SOKOL_TIME_IMPL 1

#include "deps/sokol_app.h"
#include "deps/sokol_time.h"
// --

static int gWindowWidth = 800;
static int gWindowHeight = 600;

struct MeshVtx {
    float x, y, z;
    float xn, yn, zn;
};

// ------------------------------------------------------------------------------------
// Slurp

static unsigned char* ReadSrcCode(const char* filename)
{
  FILE* in_file = 0;
  int err = fopen_s(&in_file, filename, "rb");
  assert(in_file);
  struct stat sb;
  assert(stat(filename, &sb) != -1);
  unsigned char* file_contents = (unsigned char*)malloc(sb.st_size + 1);
  if (file_contents) {
    fread(file_contents, sb.st_size, 1, in_file);
    file_contents[sb.st_size] = 0;
    fclose(in_file);
  }
  return file_contents;
}

// Shaders
// ------------------------------------------------------------------------------------

static char infoLog[512];

// Imprime los posibles errores de compilacion o linkado
static void CheckGPUErrors(unsigned int code, const char* str, bool link)
{
  int success = 0;
  if (link) {
    glGetProgramiv(code, GL_LINK_STATUS, &success);
    if (!success)
      glGetProgramInfoLog(code, 512, NULL, infoLog);
  }
  else {
    glGetShaderiv(code, GL_COMPILE_STATUS, &success);
    if (!success)
      glGetShaderInfoLog(code, 512, NULL, infoLog);
  }
  if (!success) {
    std::cout << str << infoLog << std::endl;
  }

  assert(success && "ERROR DE SHADERS");
}


static void ShadersInit(unsigned int& shader_program,
  const char* vertex_src,
  const char* shader_src)
{
  // Vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertex_src, NULL);
  glCompileShader(vertexShader);
  CheckGPUErrors(vertexShader, "VERTEX COMPILATION_FAILED\n", false);

  // Fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &shader_src, NULL);
  glCompileShader(fragmentShader);
  CheckGPUErrors(fragmentShader, "FRAGMENT COMPILATION_FAILED\n", false);

  // Link shaders
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertexShader);
  glAttachShader(shader_program, fragmentShader);
  glLinkProgram(shader_program);
  CheckGPUErrors(shader_program, "SHADER PROGRAM LINKING_FAILED\n", true);

  // Los shaders compilados pueden ser liberados una vez linkado el programa
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

// ------------------------------------------------------------------------------------
// Subir una maya a memoria grafica
// VBO = Vertex buffer object
// VAO = Vertex array object
// EBO = Element buffer object (indices)
// El objeto "array" es el que nos interesa; Los "buffer" solo sin una reservas de memoria (grafica)
// y por lo tanto los necesitaremos para la liberacion de recursos, pero no se volvera a usar

static void UploadMesh(MeshVtx* mesh, int mesh_size,
                       unsigned int* indices, int indices_size,
                       unsigned int& VBO, unsigned int& VAO, unsigned int& EBO)
{

  // Generamos 2 instancias de objetos OGL: array y buffer (sin inicializar)
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // Enfocamos ambos objetos, VAO y VBO 
  // Esto es recomendable, sino podriamos tocar atributos de VBOs y VAOs usados anteriormente
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // Configuramos el buffer (tipo y cantidad de memoria). EN ESTE MOMENTO SE COPIAN LOS VERTICES
  // STATIC_DRAW significa que los datos no se modificaran en el futuro
  glBufferData(GL_ARRAY_BUFFER, mesh_size, mesh, GL_STATIC_DRAW);
  // Idem  para los indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);
  // Configuramos el array describiendo los atributos de la estructura (posicion, normal, uv, etc)
  // Es un array de vertices de 3 floats, con una separacion entre ellos de 12 bytes
  // El indice de attributo 0 se refiere normalmente a vertices (X,Y,X),
  // el 1 a normales (X,Y,Z) y el 2 a coord. textura (U,V). Pero no esta recomendado
  // usarlos asi, sino con (ej,) glGetAttribLocation(program, "position")
  // Ademas, glVertexAttribPointer relacion el buffer con este array por el ultimo "bind"
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVtx), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVtx), (void*)12);
  glEnableVertexAttribArray(1);

  // Se quita el foco del VAO y el VBO; glVertexAttribPointer ya se ha quedado las referencias
  // Quizar el foco evita que otras llamadas puedan modificar estos objetos accidentalmente 
  // (No se debe quitar el foco del EBO mientras el VAO este en foco, o perdera la referencia)
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}


//static float gRawMesh0[3 * 3] = {
// -0.86f, -0.49f, 0.0f,
// 0.0f,  1.0f,  0.0f,
// 0.86f, -0.49f,  0.0f,
//};

static MeshVtx gMeshNormal0[4] = {
    {-0.86f, -0.49f, 0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f},
    {0.86f, -0.49f, 0.0f, 0.0f, 0.0f, 1.0f},
    {0.86f, 0.49f, 0.0f, 0.0f, 0.0f, 1.0f}
};

static MeshVtx gMeshNormal1[8] = {
    {-0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f},
    {0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f},
    {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f},
    {-0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f},
    {0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f},
    {0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f},
};


static unsigned int gIndices[3*12] = {  // note that we start from 0!
        0,4,1,
        1,4,5,

        1,5,2,
        2,5,6,

        2,6,3,
        3,6,7,
        
        3,7,0,
        0,7,4,

        3,0,2,
        2,0,1,

        6,5,4,
        6,4,7
};

static unsigned int gShaderProgram = 0;
// Mayas: Vertex buffer arrays, vertex object arrays
static unsigned int gVBO0 = 0, gVAO0 = 0, gEBO0 = 0;
static int gLocationColor = 0;

// -----------------------------------------------------------------------------------------------------------

void onInit()
{
  gladLoadGL();

  char* vertex_shader_source   = (char*)ReadSrcCode("vertex.glslv");
  char* fragment_shader_source = (char*)ReadSrcCode("fragment.glslf");

  ShadersInit(gShaderProgram, vertex_shader_source, fragment_shader_source);

  UploadMesh(gMeshNormal1, sizeof(gMeshNormal1), gIndices, sizeof(gIndices), gVBO0, gVAO0, gEBO0);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_DEPTH_TEST);

  gLocationColor = glGetUniformLocation(gShaderProgram, "RawColor");

  stm_setup(); // Init time libreria SOKOL
}

// -----------------------------------------------------------------------------------------------------------

void onFrame()
{
  gWindowWidth = sapp_width();
  gWindowHeight = sapp_height();
  double time = stm_ms(stm_now());

  // Transformaciones de objeto y perspectiva (la camara se supone fija mirando a Z)
  glm::mat4 model = glm::mat4(1.0f); 
  glm::mat4 projection = glm::mat4(1.0f);
  // Ponemos nuestra triangulo frente a la camara
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
  model = glm::rotate(model, 0.002f * (float)time, glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, 0.001f * (float)time, glm::vec3(0.0f, 1.0f, 0.0f));
  //model = glm::rotate(model, 3.1415f, glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, 0.003f * (float)time, glm::vec3(0.0f, 0.0f, 1.0f));
  // Proyeccion con un FOV de 45 grados
  projection = glm::perspective(glm::radians(45.0f), ((float)gWindowWidth) / (float)gWindowHeight, 0.1f, 100.0f);

  // Render
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(gShaderProgram);

  glUniform4f(gLocationColor, 1.0f, 1.0f, 1.0f, 1.0f);

  // Obtenemos los identificadores (location) de los "uniform" de los shaders 
  unsigned int modelLoc = glGetUniformLocation(gShaderProgram, "model");
  unsigned int projLoc = glGetUniformLocation(gShaderProgram, "projection");
  // ...y enviamos las matrices a los shaders
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

  glBindVertexArray(gVAO0);
  // Draw elements dibuja primitivas indexadas (en nuestros caso, mayas dew triangulos)
  glDrawElements(GL_TRIANGLES, sizeof(gIndices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
}

// Liberacion de recursos
// -----------------------------------------------------------------------------------------------------------

void onEnd()
{
  glDeleteVertexArrays(1, &gVAO0);
  glDeleteBuffers(1, &gVBO0);
  glDeleteBuffers(1, &gEBO0);
  glDeleteProgram(gShaderProgram);
}

// Inicializaciones: abrir ventana, etc
// La libreria Sokol exige  este nombre de funcion para la inicializacion
// ------------------------------------------------------------------------------------

sapp_desc sokol_main(int argc, char** argv)
{
  sapp_desc app = {};
  app.width = gWindowWidth;
  app.height = gWindowHeight;
  app.frame_cb = onFrame;
  app.init_cb = onInit;
  app.cleanup_cb = onEnd;
  return app;
}

