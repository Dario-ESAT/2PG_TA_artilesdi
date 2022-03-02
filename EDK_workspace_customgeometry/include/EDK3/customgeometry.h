#ifndef INCLUDE_EDK3_CUSTOM_GEOMETRY_H_
#define INCLUDE_EDK3_CUSTOM_GEOMETRY_H_ 1

// ----------------------------------------------------------------------------
// Copyright (C) 2017 Lucas Gonzalez
// Custom Geometry Class.
// Allows to create custom geometries with position and normal attributes
// ----------------------------------------------------------------------------


#include <EDK3/geometry.h>

namespace EDK3 {
  
  class Material;

  class CustomGeometry : public EDK3::Geometry {
  
  public:
	  CustomGeometry();
	  ~CustomGeometry();
	  
	  virtual bool bindAttribute(const Attribute a,  unsigned int where_to_bind_attribute) const;

	  virtual void render() const;

	  void createVertices(unsigned int size);
	  void uploadVertexPosition(float* data, unsigned int size);
	  void uploadVertexNormals(float* data, unsigned int size, unsigned int offset);

	  void createIndices(unsigned int num_indices);
	  void uploadIndices(unsigned int* faces, unsigned int size);


  private:
	  ref_ptr<dev::Buffer> vertices_;
	  ref_ptr<dev::Buffer> faces_;
	  unsigned int normal_offset_;
	  unsigned int num_elements_;
	  Type elements_type_;
  };

} /* end of EDK Namespace */

#endif
