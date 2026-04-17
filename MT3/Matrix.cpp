#include "Matrix.h"

#include <Novice.h>
#include <cmath>

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			Novice::ScreenPrintf(x + j * kColumnWidth, y + i * kRowHeigth + kRowHeigth, "%6.2f", matrix.m[i][j]);
		}
	}
}

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result;
	float augmented[4][8] = {};

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			augmented[i][j] = m.m[i][j];
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
					result.m[row][col] = 0.0f;
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
			result.m[i][j] = augmented[i][j + 4];
		}
	}

	return result;
}

Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
}

Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = (i == j) ? 1.0f : 0.0f;
		}
	}
	return result;
}
