#include "Vector&Matrix.h"

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.matrix[0][0] + vector.y * matrix.matrix[1][0] + vector.z * matrix.matrix[2][0] + matrix.matrix[3][0];
	result.y = vector.x * matrix.matrix[0][1] + vector.y * matrix.matrix[1][1] + vector.z * matrix.matrix[2][1] + matrix.matrix[3][1];
	result.z = vector.x * matrix.matrix[0][2] + vector.y * matrix.matrix[1][2] + vector.z * matrix.matrix[2][2] + matrix.matrix[3][2];
	
	result.x /= vector.x * matrix.matrix[0][3] + vector.y * matrix.matrix[1][3] + vector.z * matrix.matrix[2][3] + matrix.matrix[3][3];
	result.y /= vector.x * matrix.matrix[0][3] + vector.y * matrix.matrix[1][3] + vector.z * matrix.matrix[2][3] + matrix.matrix[3][3];
	result.z /= vector.x * matrix.matrix[0][3] + vector.y * matrix.matrix[1][3] + vector.z * matrix.matrix[2][3] + matrix.matrix[3][3];
	
	return result;
}


Matrix4x4 MakeTranslationMatrix(const Vector3& translation) {
	Matrix4x4 MakeTransla;
	MakeTransla.matrix[0][0] = 1.0f;
	MakeTransla.matrix[0][1] = 0.0f;
	MakeTransla.matrix[0][2] = 0.0f;
	MakeTransla.matrix[0][3] = 0.0f;

	MakeTransla.matrix[1][0] = 0.0f;
	MakeTransla.matrix[1][1] = 1.0f;
	MakeTransla.matrix[1][2] = 0.0f;
	MakeTransla.matrix[1][3] = 0.0f;

	MakeTransla.matrix[2][0] = 0.0f;
	MakeTransla.matrix[2][1] = 0.0f;
	MakeTransla.matrix[2][2] = 1.0f;
	MakeTransla.matrix[2][3] = 0.0f;

	MakeTransla.matrix[3][0] = translation.x;
	MakeTransla.matrix[3][1] = translation.y;
	MakeTransla.matrix[3][2] = translation.z;
	MakeTransla.matrix[3][3] = 1.0f;

	return MakeTransla;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 MakeScale;
	MakeScale.matrix[0][0] = scale.x;
	MakeScale.matrix[0][1] = 0.0f;
	MakeScale.matrix[0][2] = 0.0f;
	MakeScale.matrix[0][3] = 0.0f;
	MakeScale.matrix[1][0] = 0.0f;
	MakeScale.matrix[1][1] = scale.y;
	MakeScale.matrix[1][2] = 0.0f;
	MakeScale.matrix[1][3] = 0.0f;
	MakeScale.matrix[2][0] = 0.0f;
	MakeScale.matrix[2][1] = 0.0f;
	MakeScale.matrix[2][2] = scale.z;
	MakeScale.matrix[2][3] = 0.0f;
	MakeScale.matrix[3][0] = 0.0f;
	MakeScale.matrix[3][1] = 0.0f;
	MakeScale.matrix[3][2] = 0.0f;
	MakeScale.matrix[3][3] = 1.0f;
	return MakeScale;
}
