
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

//----

#include "dds.h"


static int gWindowWidth = 800;
static int gWindowHeight = 600;

// ------------------------------------------------------------------------------------
// Slurp

static unsigned char* ReadFile(const char* filename)
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
  assert(success);
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
// Subir una malla a memoria grafica
// VBO = Vertex buffer object
// VAO = Vertex array object
// EBO = Element buffer object (indices)
// El objeto "array" es el que nos interesa; Los "buffer" solo sin una reservas de memoria (grafica)
// y por lo tanto los necesitaremos para la liberacion de recursos, pero no se volvera a usar

struct MeshVtx
{
  float x, y, z;
  float vx, vy, vz;
  float s, t;
} ;

static void UploadMesh (const MeshVtx* mesh, int mesh_size,
                        const unsigned int* indices, int indices_size,
                        unsigned int& VBO, unsigned int& VAO, unsigned int& EBO)
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, mesh_size, mesh, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVtx), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(MeshVtx), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVtx), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}


// ------------------------------------------------------------------------------------

#define FILTROS

/*
static unsigned int UploadTexture (int w, int h)
{
  unsigned int tex = 0;
  // Creamos un "objeto" textura
  glGenTextures(1, &tex);
  // Enfocamos el objeto y le decimos que es tipo textura 2D, pero existen otros tipos relativamente 
  // habituales como textura 3d o cubemap
  glBindTexture(GL_TEXTURE_2D, tex); 
  // Esta es la configuracion tipica de "sampling" habitual, para cuando un poligono se ve lejos y cerca
#ifdef FILTROS
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#else
  // Esta seria la minima posible
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif
  // Configuramos lo que debe ocurrir cuando una textura se sale de los limites durante el pintado
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  unsigned int* argb = new unsigned int [w * h];
  int x, y;
  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      argb[x + y * w] = (x ^ y) | (x << 16)| 0xff000000;
    }
  }

  // Copia de la textura a VRAM. Params:
  // - Tipo textura (como en bindtexture)
  // - Mip map level; sirve para la alimentacion manual de mip maps
  // - Formato del pixel que queremos en VRAM 
  // - Ancho, alto
  // - Border (obsoleto. debe ser 0)
  // - Formato del pixel que le das:enumeracion de componentes (RGB, RGBA, R, G, B...)
  // - Formato de los componente del pixel que le das: char, int, float
  // - Puntero a la textura para copiar
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, argb);
  // Mip-map automatic generation
#ifdef FILTROS
  glGenerateMipmap(GL_TEXTURE_2D);
#endif
  assert(glGetError() == GL_NO_ERROR);
  return tex;
}
*/

#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT (0x83F0)

static unsigned int UploadTexture(unsigned char* dds_file)
{
  assert(dds_file[0] == 'D' && dds_file[1] == 'D' && dds_file[2] == 'S' && dds_file[3] == ' ');
  DDSHeader*      head  = (DDSHeader*)&dds_file[4];
  unsigned char* pixels = (unsigned char*)&head[1];

  //assert(GL_EXT_texture_compression_s3tc);

  unsigned int tex = 0;
  // Creamos un "objeto" textura
  glGenTextures(1, &tex);

  glBindTexture(GL_TEXTURE_2D, tex);
  // Esta es la configuracion tipica de "sampling" habitual, para cuando un poligono se ve lejos y cerca
#ifdef FILTROS
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#else
  // Esta seria la minima posible
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif
  // Configuramos lo que debe ocurrir cuando una textura se sale de los limites durante el pintado
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  int height = head->height;
  int width = head->width;
  assert(width == height);
  int size = (width * height) / 2; // DXT1 pixels 4bpp
  int i;
  for (i = 0; i < head->mipMapCount; ++i) {
    glCompressedTexImage2D(GL_TEXTURE_2D, i, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, width, height, 0, size, pixels);
    assert(glGetError() == GL_NO_ERROR);

    pixels += size;
    if (width > 4)  // En DXT las texturas no pueden ser menores a 8
      size >>= 2;    // Cada mipmap es 1/4 del anterior

    width >>= 1,    // Partimos las dimensiones a la mitad, para el siguiente mip map
    height >>= 1;
  }
  return tex;
}

// ------------------------------------------------------------------------------------

static const MeshVtx gMesh[] = {
  {-0.99f, -0.99f, -0.99f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f},
  {0.99f, -0.99f, -0.99f,  0.0f,  0.0f, -1.0f,   1.0f,  0.0f},
  {0.99f,  0.99f, -0.99f,  0.0f,  0.0f, -1.0f,   1.0f,  1.0f},
  {-0.99f,  0.99f, -0.99f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f},

  {-0.99f, -0.99f,  0.99f,  0.0f,  0.0f,  1.0f, 0.0f,  0.0f},
  {0.99f, -0.99f,  0.99f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f},
  {0.99f,  0.99f,  0.99f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f},
  {-0.99f,  0.99f,  0.99f,  0.0f,  0.0f,  1.0,  0.0f,  1.0f},

  {-0.99f,  0.99f,  0.99f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f},
  {-0.99f,  0.99f, -0.99f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f},
  {-0.99f, -0.99f, -0.99f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f},
  {-0.99f, -0.99f,  0.99f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f},

  {0.99f,  0.99f,  0.99f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f},
  {0.99f,  0.99f, -0.99f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f},
  {0.99f, -0.99f, -0.99f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f},
  {0.99f, -0.99f,  0.99f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f},

  {-0.99f, -0.99f, -0.99f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f},
  {0.99f, -0.99f, -0.99f,  0.0f, -1.0f,  0.0f,   1.0f,  1.0f},
  {0.99f, -0.99f,  0.99f,  0.0f, -1.0f,  0.0f,   1.0f,  0.0f},
  {-0.99f, -0.99f,  0.99f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f},

  {-0.99f,  0.99f, -0.99f,  0.0f,  1.0f,  0.0f, 0.0f,  1.0f},
  {0.99f,  0.99f, -0.99f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f},
  {0.99f,  0.99f,  0.99f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f},
  {-0.99f,  0.99f,  0.99f,  0.0f,  1.0f,  0.0f, 0.0f,  0.0f},
};

#define _SF(A)   A,A+1,A+2,A+2,A+3,A+0
#define _CF(A)   A,A+2,A+1,A+2,A+0,A+3   // Ad-hoc clockwise order change

static const unsigned int gIndices[] = {
  _CF(0),
  _SF(4),
  _SF(8),
  _CF(12),
  _SF(16),
  _CF(20)
};


static unsigned int gShaderProgram = 0;
// Mallas: Vertex buffer arrays, vertex object arrays
static unsigned int gVBO0 = 0, gVAO0 = 0, gEBO0 = 0;
static int gLocationColor = 0;
static int gLocationPointLightPos = 0;

static unsigned int gTextureID = 0;

// -----------------------------------------------------------------------------------------------------------

void onInit()
{
  gladLoadGL();

  char* vertex_shader_source = (char*)ReadFile("vertex.glslv");
  char* fragment_shader_source = (char*)ReadFile("fragment.glslf");
  ShadersInit(gShaderProgram, vertex_shader_source, fragment_shader_source);

  UploadMesh(gMesh, sizeof(gMesh), gIndices, sizeof(gIndices), gVBO0, gVAO0, gEBO0);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_DEPTH_TEST);

  gLocationColor = glGetUniformLocation(gShaderProgram, "RawColor");
  gLocationPointLightPos = glGetUniformLocation(gShaderProgram, "PointLightPos");

  //gTextureID = UploadTexture(256, 256);
  unsigned char* dds_data = ReadFile("data/blue_eye.dds");
  gTextureID = UploadTexture(dds_data);

  // Asignamos la "sampling unit 0" 
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gTextureID);
  unsigned int sampler_diffuse = glGetUniformLocation(gShaderProgram, "tex_diffuse");
  //glUniform1i(sampler_diffuse, 0); // Texture unit 0: textura color para luz difusa*/

  stm_setup(); // Init time libreria SOKOL

  free(vertex_shader_source);
  free(fragment_shader_source);
  free(dds_data);
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
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, -12.0f + 5.0f * sin(0.001f * (float)time)));
  model = glm::rotate(model, 0.001f * (float)time, glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, 0.0011f * (float)time, glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, 0.0014f * (float)time, glm::vec3(0.0f, 0.0f, 1.0f));
  // Proyeccion con un FOV de 45 grados
  projection = glm::perspective(glm::radians(45.0f), ((float)gWindowWidth) / (float)gWindowHeight, 0.1f, 100.0f);

  // Render
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(gShaderProgram);

  glUniform4f(gLocationColor, 0.2f, 0.2f, 1.0f, 1.0f);
  //glUniform4f(gLocationColor, 1.0f, 1.0f, 1.0f, 1.0f);
  glUniform3f(gLocationPointLightPos, 1.0f, 2.0f, -5.0f);

  // Obtenemos los identificadores (location) de los "uniform" de los shaders 
  unsigned int modelLoc = glGetUniformLocation(gShaderProgram, "model");
  unsigned int projLoc = glGetUniformLocation(gShaderProgram, "projection");
  // ...y enviamos las matrices a los shaders
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

  // Enfocar malla y textura
  glBindVertexArray(gVAO0);
  // ...esto puede ser obligatorio, activar el canal de sampleo (existen 16)
  glActiveTexture(GL_TEXTURE0); 
  glBindTexture(GL_TEXTURE_2D, gTextureID);
  // Draw elements dibuja primitivas indexadas (en nuestros caso, mallas de triangulos)
  glDrawElements(GL_TRIANGLES, sizeof(gIndices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

  assert(glGetError() == GL_NO_ERROR);
}

// Liberacion de recursos
// -----------------------------------------------------------------------------------------------------------

void onEnd()
{
  glDeleteVertexArrays(1, &gVAO0);
  glDeleteBuffers(1, &gVBO0);
  glDeleteBuffers(1, &gEBO0);
  glDeleteProgram(gShaderProgram);
  glDeleteTextures(1, &gTextureID);
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

