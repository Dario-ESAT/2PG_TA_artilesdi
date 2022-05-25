
#ifndef _OGL_TOOLS_H_
#define _OGL_TOOLS_H_

unsigned char* ReadFile(const char* filename);

unsigned int UploadShader(const char* vertex_name, const char* shader_src);

unsigned int UploadTexture(unsigned char* dds_file);

struct TFB {
  unsigned int fb;
  unsigned int tex;
};


TFB CreateRenderFB(int width, int height, GLint filter_param, GLint limits_param);

struct MeshDescriptor
{
  unsigned int VBO, VAO, EBO;
};


struct MeshVtx
{
  glm::vec3  pos;
  glm::vec2  uv;
};


struct MeshVtxTBN
{
  glm::vec3  pos;
  glm::vec2  uv;
  glm::vec3  normal;
  glm::vec3  tangent;
  //glm::vec3  bitangent; <- No es necesaria llevarla hasta el shader, ya que puede ser deducida alli 
};

MeshDescriptor UploadMesh(const MeshVtx* mesh, int mesh_size,
  const unsigned int* indices, int indices_size);


MeshDescriptor UploadMeshTBN(const MeshVtxTBN* mesh, int mesh_size,
  const unsigned int* indices, int indices_size);


#endif  // _OGL_TOOLS_H_
