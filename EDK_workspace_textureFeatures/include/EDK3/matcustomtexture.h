#pragma once
// ----------------------------------------------------------------------------
// Copyright (C) 2017 Lucas Gonzalez
// Test Material Implementation
// ----------------------------------------------------------------------------

#include "ref_ptr.h"
#include "material.h"
#include "materialsettings.h"
#include "texture.h"
#include "dev/program.h"

namespace EDK3 {

  class MatCustomTexture : public Material {
  public:
	  MatCustomTexture();
    virtual bool enable(const MaterialSettings *) const;
    virtual void setupCamera(const float projecton[16], const float view[16]) const;
    virtual void setupModel(const float m[16]) const;
	
	virtual unsigned int num_attributes_required() const;
    virtual Attribute attribute_at_index(
        const unsigned int attrib_idx) const;
    virtual Type attribute_type_at_index(
        const unsigned int attrib_index) const;

	virtual bool reloadShaders();

	void set_shader_filepaths(const char* vertex_shader, const char* fragment_shader);

    class Settings : public MaterialSettings {
    public:
      Settings()
	  { 
		  for (int i = 0; i < 4; ++i) {	
			  custom_data_[i] =01.0f;
		  }
	  }


	  // texture
	  void set_texture(Texture *tex) { texture_ = tex; }
	  Texture* texture() { return texture_.get(); }
	  const Texture* texture() const { return texture_.get(); }

	  // custom data
	  void set_custom_data(const float data[4]) { memcpy(custom_data_, data, sizeof(custom_data_)); }
	  const float *custom_data() const { return custom_data_; }	  

    protected:
      virtual ~Settings() {}
    private:
      Settings(const Settings&);
      Settings& operator=(const Settings&);
	  ref_ptr<Texture> texture_;	 
	  float custom_data_[4];
    };

  protected:
    virtual ~MatCustomTexture() {}
  private:
    ref_ptr<dev::Program> program_;
	MatCustomTexture(const MatCustomTexture&);
	MatCustomTexture& operator=(const MatCustomTexture &);
	const char* vertex_shader_;
	const char* fragment_shader_;
  };

} /* end of EDK Namespace */
