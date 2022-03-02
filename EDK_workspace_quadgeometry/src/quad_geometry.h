#ifndef QUAD_GEOMETRY
#define QUAD_GEOMETRY

#include <EDK3/geometry.h>
#include <EDK3/dev/buffer.h>

class QuadGeometry : public EDK3::Geometry
{
public:
	EDK3::ref_ptr<EDK3::dev::Buffer> vertices_;
	EDK3::ref_ptr<EDK3::dev::Buffer> indices_;

	virtual bool bindAttribute(
		const EDK3::Attribute a,
		unsigned int where_to_bind_attribute) const;

	virtual void render() const;

	QuadGeometry();
	~QuadGeometry();

private:	
};

#endif
