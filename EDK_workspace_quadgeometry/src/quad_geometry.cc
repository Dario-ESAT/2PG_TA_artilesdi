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
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	};

	//static float positions[] = {
	//	//Cara arriba
	//	-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	//	1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	//	1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	//	-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

	//	//Cara abajo
	//	-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	//	1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	//	1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	//	-1.0f, -1.0f, -1.0f,0.0f, 1.0f, 0.0f,
	//};
	static unsigned int indices[] = {
		0,2,1,
	};
	
	/*static unsigned int indices[] = {
		0,4,1,
		1,4,5,

		1,5,2,
		2,5,6,

		2,6,3,
		3,6,7,

		3,7,0,
		0,7,4,

		3,0,2,
		2,0,1,

		6,5,4,
		6,4,7
	};*/

	EDK3::dev::GPUManager::Instance()->newBuffer(&vertices_);
	EDK3::dev::GPUManager::Instance()->newBuffer(&indices_);

	vertices_->init(sizeof(float) * sizeof(positions));
	indices_->init(sizeof(float) * sizeof(indices));


	vertices_->uploadData((void*)positions, sizeof(positions));
	indices_->uploadData((void*)indices,sizeof(indices));

	vertices_->bind(EDK3::dev::Buffer::kTarget_Vertex_Data);

	bindAttribute(EDK3::Attribute::A_POSITION,0);
}

QuadGeometry::~QuadGeometry()
{	
}
