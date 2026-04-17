#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "LE1B_26";

static const int kRowHeigth = 20;
static const int kColumnWidth = 60;

struct Matrix4x4 {
	float m[4][4];
};



void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label)	 {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			Novice::ScreenPrintf(x + j * kColumnWidth, y + i * kRowHeigth + kRowHeigth, "%6.2f", matrix.m[i][j]);
		}
	}
}

Matrix4x4 add(Matrix4x4 m1,Matrix4x4 m2) {
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
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Matrix4x4 m1 = {
		3.2f ,0.7f,9.6f,4.4f,
		5.5f ,1.3f,7.8f,2.1f,
		6.9f ,8.0f,2.6f,1.0f,
		0.5f ,7.2f,5.1f,3.3f 
	};

	Matrix4x4 m2 = {
		4.1f,6.5f,3.3f,2.2f,
		8.8f,0.6f,9.9f,7.7f,
		1.1f,5.5f,6.6f,0.0f,
		3.3f,9.9f,8.8f,2.2f
	};
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Matrix4x4 resultAdd = add(m1, m2);
		Matrix4x4 resultMultiply = Multiply(m1, m2);
		Matrix4x4 resultSubtract = subtract(m1, m2);
		Matrix4x4 inverseM1 = Inverse(m1);
		Matrix4x4 inverseM2 = Inverse(m2);
		Matrix4x4 transposeM1 = Transpose(m1);
		Matrix4x4 transposeM2 = Transpose(m2);
		Matrix4x4 identity = MakeIdentity4x4();


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, 0, resultAdd, "Add");
		MatrixScreenPrintf(0, kRowHeigth*5, resultSubtract, "Subtract");
		MatrixScreenPrintf(0, kRowHeigth * 5*2, resultMultiply, "Multiply");
		MatrixScreenPrintf(0, kRowHeigth * 5*3, inverseM1, "Inverse M1");
		MatrixScreenPrintf(0, kRowHeigth * 5*4, inverseM2, "Inverse M2");
		MatrixScreenPrintf(kColumnWidth*5,0, transposeM1, "Transpose M1");
		MatrixScreenPrintf(kColumnWidth * 5, kRowHeigth * 5, transposeM2, "Transpose M2");
		MatrixScreenPrintf(kColumnWidth * 5, kRowHeigth * 5*2, identity, "Identity");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
