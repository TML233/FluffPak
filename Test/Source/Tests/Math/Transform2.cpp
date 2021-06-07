#include "doctest.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Engine/System/Math/TransformMatrix.h"
#include "Engine/System/Math/Quaternion.h"

#define CHECK_FLOAT(a,b)	\
{							\
	float diff=Math::Abs(a-b);\
	CHECK(diff < 0.000001);	\
}

#define CHECK_MATRIX(glmMatrix, engineMatrix)	\
for(int i=0;i<4;i+=1){							\
	for (int j = 0; j < 4; j += 1) {			\
		float g=glmMatrix[i][j];				\
		float e = engineMatrix.matrix[i][j];	\
		CHECK_FLOAT(g,e);						\
	}											\
}

#define CHECK_QUAT(glmQuat,engineQuat)			\
CHECK_FLOAT(glmQuat.x,engineQuat.x);			\
CHECK_FLOAT(glmQuat.y,engineQuat.y);			\
CHECK_FLOAT(glmQuat.z,engineQuat.z);			\
CHECK_FLOAT(glmQuat.w,engineQuat.w)			

using namespace Engine;
TEST_CASE("Matrix") {
	glm::mat4 gm0{ 1 };
	TransformMatrix em0{};
	CHECK_MATRIX(gm0, em0);

	glm::mat4 gm1 = glm::mat4(1);
	gm1 = glm::translate(gm1, glm::vec3(1, 2, 3));
	TransformMatrix em1 = TransformMatrix::Translate(Vector3(1, 2, 3));
	CHECK_MATRIX(gm1, em1);

	glm::mat4 gm2 = glm::mat4(1);
	gm2 = glm::scale(gm2, glm::vec3(1, 2, 3));
	TransformMatrix em2 = TransformMatrix::Scale(Vector3(1, 2, 3));
	CHECK_MATRIX(gm2, em2);

	glm::quat gq0 = glm::quat(glm::vec3(0.f, 0.f, 0.f));
	Quaternion eq0 = Quaternion();
	CHECK_QUAT(gq0, eq0);

	glm::quat gq1 = glm::quat(glm::vec3(0.f, 0.f, 45.f));
	Quaternion eq1 = Quaternion::FromAxisAngle(Vector3(0, 0, 1), 45);
	CHECK_QUAT(gq1, eq1);

	glm::mat4 gqm1 = glm::mat4_cast(gq1);
	TransformMatrix eqm1 = eq1.ToTransformMatrix();
	CHECK_MATRIX(gqm1, eqm1);

	glm::mat4 gm3 = glm::mat4(1);
	gm3 = glm::rotate(gm3, 20.0f, glm::vec3(0.0f, 0.0f, 1.f));
	TransformMatrix em3 = TransformMatrix::Rotate(Vector3(0.0f, 0.0f, 1.f), 20.f);
	CHECK_MATRIX(gm3, em3);

	glm::mat4 gmm = gm2 * gm1;
	TransformMatrix emm = em2 * em1;
	CHECK_MATRIX(gmm, emm);


}