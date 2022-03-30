#include "quad_geometry.h"
#include "EDK3/dev/gpumanager.h"

//layouts - usar bind del buffer
bool QuadGeometry::bindAttribute(const EDK3::Attribute attribute, unsigned int where_to_bind_attribute) const
{
	if (attribute == EDK3::Attribute::A_POSITION) {
		EDK3::dev::GPUManager::Instance()->enableVertexAttribute(
			vertices_.get(),
			where_to_bind_attribute,
			EDK3::Type::T_FLOAT_3
		);
	} else if (attribute == EDK3::Attribute::A_NORMAL) {
		EDK3::dev::GPUManager::Instance()->enableVertexAttribute(
			vertices_.get(),
			where_to_bind_attribute,
			EDK3::Type::T_FLOAT_3,
			false,
			12
		);
	} else if(attribute == EDK3::Attribute::A_UV){
		EDK3::dev::GPUManager::Instance()->enableVertexAttribute(
			vertices_.get(),
			where_to_bind_attribute,
			EDK3::Type::T_FLOAT_2,
			false,
			24
		);
	}
	return true;
}

//gpu manager - drawElements
void QuadGeometry::render() const {

	EDK3::dev::GPUManager::Instance()->drawElements(
		EDK3::dev::GPUManager::DrawMode::kDrawMode_Triangles,
		3,
		indices_.get(),
		EDK3::T_UINT
	);
}

//  hacer los buffers de vertices e indices
QuadGeometry::QuadGeometry() {
	
	static float positions[] = {
		//Cara arriba
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
	};

	static const MeshVtx gMesh[] = {
	{-1.0f, -1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f},
	{1.0f, -1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f},
	{1.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f},
	{-1.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f},
	};
#define _SF(A)   A,A+1,A+2,A+2,A+3,A+0
#define _CF(A)   A,A+2,A+1,A+2,A+0,A+3   // Ad-hoc clockwise order change

	static unsigned int indices[] = {
		//0,1,2,
		//0,2,3,
		//_SF(0),
		_CF(0),
	};
	


	EDK3::dev::GPUManager::Instance()->newBuffer(&vertices_);
	EDK3::dev::GPUManager::Instance()->newBuffer(&indices_);

	vertices_->init(sizeof(float) * sizeof(positions));
	indices_->init(sizeof(float) * sizeof(indices));


	vertices_->uploadData((void*)positions, sizeof(positions));

	indices_->uploadData((void*)indices,sizeof(indices));

	vertices_->bind(EDK3::dev::Buffer::kTarget_Vertex_Data);

	bindAttribute(EDK3::Attribute::A_POSITION, 0);
	//bindAttribute(EDK3::Attribute::A_NORMAL, 1);
	//bindAttribute(EDK3::Attribute::A_UV,2);
}

QuadGeometry::~QuadGeometry()
{	
}
