#pragma once
#include <cmath>

struct Matrix4x4 {
	float matrix[4][4];
};
constexpr int kRowHeight = 20;
constexpr int kColumnWidth = 60;


void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);
Matrix4x4 Add(const Matrix4x4& matrix1, const Matrix4x4& matrix2);
Matrix4x4 Subtract(const Matrix4x4& matrix1, const Matrix4x4& matrix2);
Matrix4x4 Multiply(const Matrix4x4& matrix1, const Matrix4x4& matrix2);
Matrix4x4 Inverse(const Matrix4x4& matrix);
Matrix4x4 Transpose(const Matrix4x4& matrix);
Matrix4x4 MakeIdentity4x4();

//1 x軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);


//2 y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);

//3 z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);