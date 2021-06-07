#include "doctest.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Engine/System/Math/TransformMatrix.h"

#define CHECK_MATRIX(glmMatrix, engineMatrix)	\
for(int i=0;i<4;i+=1){							\
	for (int j = 0; j < 4; j += 1) {			\
		float g=glmMatrix[i][j];			\
		float e = engineMatrix.matrix[i][j];\
		CHECK(g == e);\
	}											\
}
using namespace Engine;
TEST_CASE("Matrix") {
	glm::mat4 gm{ 1 };
	TransformMatrix em{};
	CHECK_MATRIX(gm, em);

	gm = glm::translate(gm, glm::vec3(1, 2, 3));
	em = TransformMatrix::Translate(Vector3(1, 2, 3));
	CHECK_MATRIX(gm, em);
}