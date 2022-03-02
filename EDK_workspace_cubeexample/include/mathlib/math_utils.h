// @author Enrique Gil <gilte@esat-alumni.com>
// @file math_utils header implementation

#ifndef __MATHUTILS_H__
#define __MATHUTILS_H__ 1

namespace matlib {

class MathUtils {
 public:
	static float MathUtils::Clamp(float value, float minVal, float maxVal);

 private:
	MathUtils();
	MathUtils(const MathUtils& copy);
	~MathUtils();
};

}

#endif