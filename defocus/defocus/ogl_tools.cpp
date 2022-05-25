
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

#include "deps/glad.h"

#include "dds.h"
#include "ogl_tools.h"

// ------------------------------------------------------------------------------------
// Slurp

unsigned char* ReadFile(const char* filename)
{
  FILE* in_file = 0;
  int err = fopen_s(&in_file, filename, "rb");
  assert(in_file);
  struct stat sb;
  assert(stat(filename, &sb) != -1);
  unsigned char* file_contents = new unsigned char[sb.st_size + 1];
  if (file_contents) {
    fread(file_contents, sb.st_size, 1, in_file);
    file_contents[sb.st_size] = 0;
    fclose(in_file);
  }
  return file_contents;
}

// ------------------------------------------------------------------------------------

#define FILTROS

#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT (0x83F0)

unsigned int UploadTexture(unsigned char* dds_file)
{
  assert(dds_file[0] == 'D' && dds_file[1] == 'D' && dds_file[2] == 'S' && dds_file[3] == ' ');
  DDSHeader* head = (DDSHeader*)&dds_file[4];
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
  for (i = 0; i < (signed)head->mipMapCount; ++i) {
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


TFB CreateRenderFB(int width, int height, GLint filter_param, GLint limits_param)
{
  TFB tfb = {0,0};

  // Framebuffer para renderizar indirectamente la escena
  glGenFramebuffers(1, &tfb.fb);
  glBindFramebuffer(GL_FRAMEBUFFER, tfb.fb);
  // Creamos una textura; al ser del tamaño de una ventana, no va a ser potencia de 2
  // 
  glGenTextures(1, &tfb.tex);
  glBindTexture(GL_TEXTURE_2D, tfb.tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  // La configuracion de la textura para los samplers es algo diferente de lo habitual:
  //  - no hay mipmaps: solo hemos renderizado un frame, sin miniaturas
  //  - no se lee como un patron repetitivo; si una lectura se sale, la limitamos al borde
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_param);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_param);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, limits_param);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, limits_param);
  // Attach/bind de la textura al framebuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tfb.tex, 0);
  // Para poder renderizar, hacen falta buffers extra como z buffer o stencil
  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  // Creamos un z buffer
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, width, height);
  // Lo asociamos al frame buffer (fb, que estaba bindeado)
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
  // Comprobamos que esta completo
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    fprintf(stderr, "La creacion del framebuffer esta incompleta\n");
    exit(1);
  }
  // Obtener el id del zbuffer par poder usarlo como textura en postproces
  //GLint zbuf = -1;
  //glGetNamedFramebufferAttachmentParameteriv(gTextureFB.fb, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &zbuf);

  // ...desbindeamos
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  assert(glGetError() == GL_NO_ERROR);
  return tfb;
}

// ------------------------------------------------------------------------------------


MeshDescriptor UploadMesh(const MeshVtx* mesh, int mesh_size,
                          const unsigned int* indices, int indices_size)
{
  MeshDescriptor M = {0,0,0};
  glGenVertexArrays(1, &M.VAO);
  glGenBuffers(1, &M.VBO);
  glGenBuffers(1, &M.EBO);

  glBindVertexArray(M.VAO);
  glBindBuffer(GL_ARRAY_BUFFER, M.VBO);

  glBufferData(GL_ARRAY_BUFFER, mesh_size, mesh, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVtx), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVtx), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return M;
}

MeshDescriptor UploadMeshTBN(const MeshVtxTBN* mesh, int mesh_size,
                             const unsigned int* indices, int indices_size)
{
  MeshDescriptor M = {0,0,0};
  glGenVertexArrays(1, &M.VAO);
  glGenBuffers(1, &M.VBO);
  glGenBuffers(1, &M.EBO);

  glBindVertexArray(M.VAO);
  glBindBuffer(GL_ARRAY_BUFFER, M.VBO);

  glBufferData(GL_ARRAY_BUFFER, mesh_size, mesh, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVtxTBN), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVtxTBN), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVtxTBN), (void*)(5 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVtxTBN), (void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return M;
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


unsigned int UploadShader(const char* vertex_name, const char* fragment_name)
{
  char* vertex_src = (char*)ReadFile(vertex_name);
  char* fragment_src = (char*)ReadFile(fragment_name);

  unsigned int shader_program = 0;
  // Vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertex_src, NULL);
  glCompileShader(vertexShader);
  CheckGPUErrors(vertexShader, "VERTEX COMPILATION_FAILED\n", false);

  // Fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragment_src, NULL);
  glCompileShader(fragmentShader);
  CheckGPUErrors(fragmentShader, "FRAGMENT COMPILATION_FAILED\n", false);

  // Link shaders
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertexShader);
  glAttachShader(shader_program, fragmentShader);
  glLinkProgram(shader_program);
  CheckGPUErrors(shader_program, "SHADER PROGRAM LINKING_FAILED\n", true);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  delete(vertex_src);
  delete(fragment_src);
  return shader_program;
}