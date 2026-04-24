#include "Matrix.h"

#include <Novice.h>
#include <cmath>

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			Novice::ScreenPrintf(x + j * kColumnWidth, y + i * kRowHeight + kRowHeight, "%6.2f", matrix.matrix[i][j]);
		}
	}
}

Matrix4x4 Add(const Matrix4x4& matrix1, const Matrix4x4& matrix2){
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.matrix[i][j] = matrix1.matrix[i][j] + matrix2.matrix[i][j];
		}
	}
	return result;
}
Matrix4x4 Subtract(const Matrix4x4& matrix1, const Matrix4x4& matrix2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.matrix[i][j] = matrix1.matrix[i][j] - matrix2.matrix[i][j];
		}
	}
	return result;
}

Matrix4x4 Multiply(const Matrix4x4& matrix1, const Matrix4x4& matrix2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.matrix[i][j] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				result.matrix[i][j] += matrix1.matrix[i][k] * matrix2.matrix[k][j];
			}
		}
	}
	return result;
}

Matrix4x4 Inverse(const Matrix4x4& matrix) {
	Matrix4x4 result;
	float augmented[4][8] = {};

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			augmented[i][j] = matrix.matrix[i][j];
			augmented[i][j + 4] = (i == j) ? 1.0f : 0.0f;
		}
	}

	for (int i = 0; i < 4; ++i) {
		int pivotRow = i;
		for (int j = i + 1; j < 4; ++j) {
			if (std::fabs(augmented[j][i]) > std::fabs(augmented[pivotRow][i])) {
				pivotRow = j;
			}
		}

		if (std::fabs(augmented[pivotRow][i]) < 1.0e-6f) {
			for (int row = 0; row < 4; ++row) {
				for (int col = 0; col < 4; ++col) {
					result.matrix[row][col] = 0.0f;
				}
			}
			return result;
		}

		if (pivotRow != i) {
			for (int j = 0; j < 8; ++j) {
				float temp = augmented[i][j];
				augmented[i][j] = augmented[pivotRow][j];
				augmented[pivotRow][j] = temp;
			}
		}

		float pivot = augmented[i][i];
		for (int j = 0; j < 8; ++j) {
			augmented[i][j] /= pivot;
		}

		for (int row = 0; row < 4; ++row) {
			if (row == i) {
				continue;
			}

			float factor = augmented[row][i];
			for (int col = 0; col < 8; ++col) {
				augmented[row][col] -= factor * augmented[i][col];
			}
		}
	}

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.matrix[i][j] = augmented[i][j + 4];
		}
	}

	return result;
}

Matrix4x4 Transpose(const Matrix4x4& matrix) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.matrix[i][j] = matrix.matrix[j][i];
		}
	}
	return result;
}

Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.matrix[i][j] = (i == j) ? 1.0f : 0.0f;
		}
	}
	return result;
}


Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result={0};
	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	result.matrix[1][1] = cosTheta;
	result.matrix[1][2] = -sinTheta;
	result.matrix[2][1] = sinTheta;
	result.matrix[2][2] = cosTheta;
	result.matrix[0][0] = 1.0f;
	result.matrix[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4  result = { 0 };
	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	result.matrix[0][0] = cosTheta;
	result.matrix[0][2] = sinTheta;
	result.matrix[2][0] = -sinTheta;
	result.matrix[2][2] = cosTheta;
	result.matrix[1][1] = 1.0f;
	result.matrix[3][3] = 1.0f;
	return result;
}


Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4  result = { 0 };
	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	result.matrix[0][0] = cosTheta;
	result.matrix[0][1] = -sinTheta;
	result.matrix[1][0] = sinTheta;
	result.matrix[1][1] = cosTheta;
	result.matrix[2][2] = 1.0f;
	result.matrix[3][3] = 1.0f;
	return result;
}