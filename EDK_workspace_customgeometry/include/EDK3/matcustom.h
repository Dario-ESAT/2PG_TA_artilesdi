#pragma once
// ----------------------------------------------------------------------------
// Copyright (C) 2017 Lucas Gonzalez
// Test Material Implementation
// ----------------------------------------------------------------------------

#include "ref_ptr.h"
#include "material.h"
#include "materialsettings.h"	
#include "dev/program.h"

namespace EDK3 {

  class MatCustom : public Material {
  public:
	MatCustom();
    virtual bool enable(const MaterialSettings *) const;
    virtual void setupCamera(const float projecton[16], const float view[16]) const;
    virtual void setupModel(const float m[16]) const;
	
	virtual unsigned int num_attributes_required() const;
    virtual Attribute attribute_at_index(
        const unsigned int attrib_idx) const;
    virtual Type attribute_type_at_index(
        const unsigned int attrib_index) const;

	virtual bool load_shaders_from_filepaths(const char* vertex_shader_path, const char* fragment_shader_path);
	virtual bool load_shaders(ref_ptr<dev::Shader> vertex_shader, ref_ptr<dev::Shader> fragment_shader);

    class Settings : public MaterialSettings {
    public:
      Settings()
	  { 
		  for (int i = 0; i < 4; ++i) {
			  color_data[i] = 1.0f;
			  custom_data_[i] = 1.0f;
		  }
	  }
	  void set_color(const float v[4]) { memcpy(color_data, v, sizeof(color_data)); }
      void set_color(float r, float g, float b, float a = 1.0f) {
		  color_data[0] = r; color_data[1] = g; color_data[2] = b; color_data[3] = a;
      }
      const float *color() const { return color_data; }

	  void set_custom_data(const float data[4]) { memcpy(custom_data_, data, sizeof(custom_data_)); }
	  const float *custom_data() const { return custom_data_; }

    protected:
      virtual ~Settings() {}

    private:
      Settings(const Settings&);
      Settings& operator=(const Settings&);
	  float color_data[4];
	  float custom_data_[4];
    };


  protected:
	  dev::Program* get_program();
	  const dev::Program* get_program() const;

    virtual ~MatCustom() {}
  private:
    ref_ptr<dev::Program> program_;
	MatCustom(const MatCustom&);
	MatCustom& operator=(const MatCustom &);
  };

} /* end of EDK Namespace */
