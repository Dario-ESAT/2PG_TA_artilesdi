
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

#define SOKOL_GLCORE33 1
#define SOKOL_APP_IMPL 1
#define SOKOL_TIME_IMPL 1

#include "deps/sokol_app.h"
#include "deps/sokol_time.h"

#include "ogl_tools.h"



static int gWindowWidth = 800;
static int gWindowHeight = 600;


// ------------------------------------------------------------------------------------


static const MeshVtx gCubeVtx[] = {
  {{-0.99f, -0.99f, -0.99f}, {0.0f,  0.0f}},
  {{0.99f, -0.99f, -0.99f},  {1.0f,  0.0f}},
  {{0.99f,  0.99f, -0.99f},  {1.0f,  1.0f}},
  {{-0.99f,  0.99f, -0.99f}, {0.0f,  1.0f}},

  {{-0.99f, -0.99f,  0.99f},  {0.0f,  0.0f}},
  {{0.99f, -0.99f,  0.99f},   {1.0f,  0.0f}},
  {{0.99f,  0.99f,  0.99f},   {1.0f,  1.0f}},
  {{-0.99f,  0.99f,  0.99f},  {0.0f,  1.0f}},

  {{-0.99f,  0.99f,  0.99f},  {1.0f,  0.0f}},
  {{-0.99f,  0.99f, -0.99f},  {1.0f,  1.0f}},
  {{-0.99f, -0.99f, -0.99f},  {0.0f,  1.0f}},
  {{-0.99f, -0.99f,  0.99f}, { 0.0f,  0.0f}},

  {{0.99f,  0.99f,  0.99f},  {1.0f,  0.0f}},
  {{0.99f,  0.99f, -0.99f},  {1.0f,  1.0f}},
  {{0.99f, -0.99f, -0.99f},  {0.0f,  1.0f}},
  {{0.99f, -0.99f,  0.99f},  {0.0f,  0.0f}},

  {{-0.99f, -0.99f, -0.99f},  {0.0f,  1.0f}},
  {{0.99f, -0.99f, -0.99f},   {1.0f,  1.0f}},
  {{0.99f, -0.99f,  0.99f},   {1.0f,  0.0f}},
  {{-0.99f, -0.99f,  0.99f},  {0.0f,  0.0f}},

  {{-0.99f,  0.99f, -0.99f},  {0.0f,  1.0f}},
  {{ 0.99f,  0.99f, -0.99f}, {1.0f, 1.0f}},
  {{0.99f,  0.99f,  0.99f}, {1.0f, 0.0f}},
  {{-0.99f,  0.99f,  0.99f}, {0.0f, 0.0f}},
};

#define _SF(A)   A,A+1,A+2,A+2,A+3,A+0
#define _CF(A)   A,A+2,A+1,A+2,A+0,A+3   // Ad-hoc clockwise order change

#define TF(N) ((1.0f/273.0f) * (float)N)

static const unsigned int gCubeIndices[] = {
  _CF(0),
  _SF(4),
  _SF(8),
  _CF(12),
  _SF(16),
  _CF(20)
};

#define UN (1.0f)
static const MeshVtx gQuadVtx[] = { 
  {{-UN,  UN, 0.0f}, {0.0f, 1.0f}},
  {{-UN, -UN, 0.0f}, {0.0f, 0.0f}},
  {{ UN, -UN, 0.0f}, {1.0f, 0.0f}},
  {{ UN,  UN, 0.0f}, {1.0f, 1.0f}}
};

static const unsigned int gQuadIndices[] = {
  0,2,3,
  0,1,2,

};

static const float gaussM[] = {
  TF(1),TF(4), TF(7) ,TF(4), TF(1),
  TF(4),TF(16),TF(26),TF(16),TF(4),
  TF(7),TF(26),TF(41),TF(26),TF(7),
  TF(4),TF(16),TF(26),TF(16),TF(4),
  TF(1),TF(4), TF(7), TF(4), TF(1),
};


static unsigned int gProgramNMap = 0;
static unsigned int gProgramBlit = 0;
// Mallas
static MeshDescriptor gCube = {0,0,0};
static MeshDescriptor gQuad = {0,0,0};

static int gLocationPointLightPos = 0;

static unsigned int gTexDiffuse = -1;
static unsigned int gTexNormals = -1;

static TFB gTextureFB;

static MeshVtxTBN* gCubeVtxTBN = 0;

// -----------------------------------------------------------------------------------------------------------

void onInit()
{
  gladLoadGL();

  // Convertir la maya a un nuevo formato añadiendo los vectores Normal-Tangent
  // Esta simplificacion solo funciona con esta mayas de QUADS, que no tiene vertices compartidos
  int nvertex = sizeof(gCubeVtx) / sizeof(MeshVtx);
  gCubeVtxTBN = new MeshVtxTBN[nvertex];

  int face = 0;
  for (int i = 0; i < nvertex; i+=4, face++) {
    int i0 = gCubeIndices[face * 6 + 0];
    int i1 = gCubeIndices[face * 6 + 1];
    int i2 = gCubeIndices[face * 6 + 2];
    // Sacamos vectores de un punto del quad respecto a 2 vecinos (cualesquiera)
    glm::vec3 edge1 = gCubeVtx[i1].pos - gCubeVtx[i0].pos;
    glm::vec3 edge2 = gCubeVtx[i2].pos - gCubeVtx[i0].pos;
    glm::vec2 deltaUV1 = gCubeVtx[i1].uv - gCubeVtx[i0].uv;
    glm::vec2 deltaUV2 = gCubeVtx[i2].uv - gCubeVtx[i0].uv;
    // Obtenemos la normal
    glm::vec3 N = glm::normalize(glm::cross(edge1, edge2));
    // Calculo de la tangente
    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
    glm::vec3 T(0.0f,0.0f,0.0f);
    T.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    T.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    T.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    T = glm::normalize(T);

    for (int j = i; j < i + 4; j++) {
      // Copiamos las posiciones de los vertices del quad
      gCubeVtxTBN[j].pos = gCubeVtx[j].pos;
      gCubeVtxTBN[j].uv = gCubeVtx[j].uv;
      // Usaremos la misma matriz TBN para todo el quad, ya que los vertices no son compartidos con otros poligonos
      gCubeVtxTBN[j].normal = N;
      gCubeVtxTBN[j].tangent = T;
    }
  }

  gCube = UploadMeshTBN(gCubeVtxTBN, nvertex * sizeof(MeshVtxTBN), gCubeIndices, sizeof(gCubeIndices));

  gQuad = UploadMesh(gQuadVtx, sizeof(gQuadVtx), gQuadIndices, sizeof(gQuadIndices));

  // Shaders
  gProgramNMap = UploadShader("vertex_nmap.glslv", "fragment_nmap.glslf");
  gProgramBlit = UploadShader("vertex_blit.glslv", "fragment_blit.glslf");

  // Textura

  glUseProgram(gProgramNMap);
  gLocationPointLightPos = glGetUniformLocation(gProgramNMap, "PointLightPos");
  // Texture samplers
  unsigned int sampler_diffuse = glGetUniformLocation(gProgramNMap, "tex_diffuse");
  unsigned int sampler_normals = glGetUniformLocation(gProgramNMap, "tex_normals");
  glUniform1i(sampler_diffuse, 0); // Texture unit 0: textura color para luz difusa
  glUniform1i(sampler_normals, 1); // Texture unit 1: normal map

  unsigned char* dds_diffuse = ReadFile("data/marble.dds");
  gTexDiffuse = UploadTexture(dds_diffuse);
  unsigned char* dds_normals = ReadFile("data/wavy_rb.dds");
  gTexNormals = UploadTexture(dds_normals);

  // Creamos un render target
  //gTextureFB = CreateRenderFB(gWindowWidth, gWindowHeight, GL_NEAREST, GL_CLAMP_TO_EDGE);
  gTextureFB = CreateRenderFB(gWindowWidth, gWindowHeight, GL_LINEAR, GL_CLAMP_TO_EDGE);

  glUseProgram(gProgramBlit);
  unsigned int postprocLoc = glGetUniformLocation(gProgramBlit, "postproc_image");
  glUniform1i(postprocLoc, 0); // Texture unit 0: texture render target

  // Timer
  stm_setup(); // Init time libreria SOKOL


  delete dds_diffuse;
  delete dds_normals;
}

// -----------------------------------------------------------------------------------------------------------

static const glm::vec3 gCubePos[] =
{
  {-1.0f, -1.0f, -3.0f},
  {0.0f, 0.0f, -7.0f},
  {3.0f, 0.0f, -12.0f},
  {-4.0f, 4.0f, -15.0f},
  {5.0f, 5.0f, -13.0f}
};

void onFrame()
{
  gWindowWidth = sapp_width();
  gWindowHeight = sapp_height();
  double time = stm_ms(stm_now());

  // Proyeccion con un FOV de 45 grados
  glm::mat4 projection = glm::mat4(1.0f);
  projection = glm::perspective(glm::radians(45.0f), ((float)gWindowWidth) / (float)gWindowHeight, 0.1f, 100.0f);

  // bind to framebuffer and draw scene as we normally would to color texture 
  glBindFramebuffer(GL_FRAMEBUFFER, gTextureFB.fb);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

  // Render
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (int i = 0; i < sizeof(gCubePos) / sizeof(glm::vec3); i++) {
    // Transformaciones de objeto y perspectiva (la camara se supone fija mirando a Z)
    glm::mat4 model = glm::mat4(1.0f);
    // Ponemos nuestro objeto frente a la camara
    model = glm::translate(model, gCubePos[i]);
    float obj_time = (float)(time + (double)(i * 1000));
    model = glm::rotate(model, 0.001f * obj_time, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 0.0011f * obj_time, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, 0.0014f * obj_time, glm::vec3(0.0f, 0.0f, 1.0f));
 
    glUseProgram(gProgramNMap);

    glUniform3f(gLocationPointLightPos, 1.0f, 2.0f, -5.0f);

    // Obtenemos los identificadores (location) de los "uniform" de los shaders 
    unsigned int modelLoc = glGetUniformLocation(gProgramNMap, "model");
    unsigned int projLoc = glGetUniformLocation(gProgramNMap, "projection");
    unsigned int gussmLoc = glGetUniformLocation(gProgramNMap, "gaussM");

    // ...y enviamos las matrices a los shaders
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
    glUniform1fv(gussmLoc, 25, gaussM);
    // Enfocar malla y textura
    glBindVertexArray(gCube.VAO);
    // Activar el canal de sampleo (existen 16)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexDiffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gTexNormals);
    // Draw elements dibuja primitivas indexadas (en nuestros caso, mallas de triangulos)
    glDrawElements(GL_TRIANGLES, sizeof(gCubeIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

    assert(glGetError() == GL_NO_ERROR);
  }

  // Volvemos al framebuffer default
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glUseProgram(gProgramBlit);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(gQuad.VAO);
  // Usamos la textura que habiamos reservado asociada al nuevo framebuffer 
  glBindTexture(GL_TEXTURE_2D, gTextureFB.tex);	

  glDrawElements(GL_TRIANGLES, sizeof(gQuadIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

  assert(glGetError() == GL_NO_ERROR);
}

// Liberacion de recursos
// -----------------------------------------------------------------------------------------------------------

void onEnd()
{
  glDeleteVertexArrays(1, &gCube.VAO);
  glDeleteBuffers(1, &gCube.VBO);
  glDeleteBuffers(1, &gCube.EBO);
  glDeleteProgram(gProgramNMap);
  glDeleteTextures(1, &gTexDiffuse);
  glDeleteTextures(1, &gTexNormals);
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
  //app.win32_console_create = true;
  app.win32_console_attach = true;
  return app;
}

