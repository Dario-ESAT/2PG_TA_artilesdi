#ifndef __SuperMaterial
#define __SuperMaterial 1

// ----------------------------------------------------------------------------
// Copyright (C) 2015 Jose L. Hidalgo 
// Material Class.
// ----------------------------------------------------------------------------

#include "EDK3/referenced.h"
#include "EDK3/constants.h"
#include "EDK3/ref_ptr.h"
#include "EDK3/matcustom.h"


  class SuperMaterial : public EDK3::MatCustom {
  public:

    SuperMaterial();
    ~SuperMaterial(); 
    SuperMaterial(const SuperMaterial&);
    virtual bool enable(const EDK3::MaterialSettings*) const override;
    virtual void setupCamera(const float projecton[16], const float view[16]) const;
    virtual void setupModel(const float m[16]) const override;

    virtual unsigned int num_attributes_required() const override;
    virtual EDK3::Attribute attribute_at_index(
        const unsigned int attrib_idx) const override;
    virtual EDK3::Type attribute_type_at_index(
        const unsigned int attrib_index) const override;


    bool load_shaders(EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader, EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader) override;

  };

#endif
