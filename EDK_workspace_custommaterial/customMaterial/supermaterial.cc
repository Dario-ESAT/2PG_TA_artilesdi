#include "supermaterial.h"
#include "EDK3/dev/shader.h"
#include "EDK3/dev/gpumanager.h"
#include "EDK3/dev/program.h"
#include "mathlib/mathlib.h"

SuperMaterial::SuperMaterial() {}

SuperMaterial::~SuperMaterial() {}

SuperMaterial::SuperMaterial(const SuperMaterial&) {}

bool SuperMaterial::enable(const EDK3::MaterialSettings*) const {
    get_program()->use();
    return true;
}

void SuperMaterial::setupCamera(const float projecton[16], const float view[16]) const {
    int uniformID = get_program()->get_uniform_position("u_vp_matrix");

    matlib::Matrix4x4 p_m(projecton);
    matlib::Matrix4x4 v_m(view);
    matlib::Matrix4x4 vp_m(v_m.Multiply(p_m));

    get_program()->set_uniform_value(uniformID, EDK3::T_MAT_4x4, vp_m.m);
}

void SuperMaterial::setupModel(const float m[16]) const {
    int uniformID = get_program()->get_uniform_position("u_m_matrix");
    get_program()->set_uniform_value(uniformID, EDK3::T_MAT_4x4, m);
}

unsigned int SuperMaterial::num_attributes_required() const {

    return 2;
}

EDK3::Attribute SuperMaterial::attribute_at_index(const unsigned int attrib_idx) const {
    if (attrib_idx == 0) return EDK3::Attribute::A_POSITION;
    if (attrib_idx == 1) return EDK3::Attribute::A_NORMAL;
    if (attrib_idx > 1) return EDK3::Attribute::A_NONE;
}

EDK3::Type SuperMaterial::attribute_type_at_index(const unsigned int attrib_index) const {
    if (attrib_index == 0) return EDK3::Type::T_FLOAT_3;
    if (attrib_index == 1) return EDK3::Type::T_FLOAT_3;
    if (attrib_index > 1) return EDK3::Type::T_NONE;
}

bool SuperMaterial::load_shaders(EDK3::ref_ptr<EDK3::dev::Shader> vertex_shader, EDK3::ref_ptr<EDK3::dev::Shader> fragment_shader) {
    if (!vertex_shader->compile()) return false;
    if (!fragment_shader->compile()) return false;

    EDK3::ref_ptr<EDK3::dev::Program> program;
    EDK3::dev::GPUManager::Instance()->newProgram(&program);
    program->attach(vertex_shader.get());
    program->attach(fragment_shader.get());

    program->link();

    set_program(program);
    return true;
}

