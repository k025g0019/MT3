


#include "Novice.h"
#include "Vector.h"
#include  "math.h"
void VectorScreenPrintf(int x, int y, Vector3 result, const char* label) {
	Novice::ScreenPrintf(x, y, "%0.02f", result.x);
	Novice::ScreenPrintf(x + 50, y, " %0.02f", result.y);
	Novice::ScreenPrintf(x + 100, y, "%0.02f %s", result.z, label);
}

//加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;

}

//減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

//スカラー倍
Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 result;
	result.x = scalar * v.x;
	result.y = scalar * v.y;
	result.z = scalar * v.z;
	return result;
}

//内積
float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//ベクトルの長さ
float Length(const Vector3& v) {
	return sqrt(Dot(v, v));
}

//正規化
Vector3 Normalize(const Vector3& v) {
	float length = Length(v);
	if (length == 0.0f) {
		return { 0.0f, 0.0f, 0.0f }; // ゼロベクトルの正規化はゼロベクトルを返す
	}
	return Multiply(1.0f / length, v);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			Novice::ScreenPrintf(x + j * kColumnWidth, y + i * kRowHeigth + kRowHeigth, "%6.2f", matrix.m[i][j]);
		}
	}
}

Matrix4x4 add(Matrix4x4 m1, Matrix4x4 m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return result;
}

Matrix4x4 subtract(Matrix4x4 m1, Matrix4x4 m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
}


Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2) {
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

Matrix4x4 Inverse(Matrix4x4 m1) {
	Matrix4x4 result;
	float augmented[4][8] = {};

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			augmented[i][j] = m1.m[i][j];
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
				float tmp = augmented[i][j];
				augmented[i][j] = augmented[pivotRow][j];
				augmented[pivotRow][j] = tmp;
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

Matrix4x4 Transpose(Matrix4x4 m1) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[j][i];
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
