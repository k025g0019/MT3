#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "LE1B_26";

struct Vector3 {
	float x;
	float y;
	float z;
};

void VectorScreenPrintf(int x, int y, Vector3 result, const char* label) {
	Novice::ScreenPrintf(x, y, "%0.02f", result.x);
	Novice::ScreenPrintf(x+50, y, " %0.02f", result.y);
	Novice::ScreenPrintf(x+100, y, "%0.02f %s", result.z, label);
}

//加算
Vector3 Add(const Vector3& v1 , const Vector3& v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;

}

//減算
Vector3 Subtract(const Vector3& v1,const Vector3& v2) {
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
		return {0.0f, 0.0f, 0.0f}; // ゼロベクトルの正規化はゼロベクトルを返す
	}
	return Multiply(1.0f / length, v);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};
	Vector3 v1 = {1.0f, 3.0f, -5.0f};
	Vector3 v2 = { 4.0f, -1.0f, 2.0f };
	float k = 4.0f;

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

		Vector3 resultAdd = Add(v1, v2);
		Vector3 resultSubtract = Subtract(v1, v2);
		Vector3 resultMultiply = Multiply(k, v1);
		float resultDot = Dot(v1, v2);
		float resultLength = Length(v1);
		Vector3 resultNormalize = Normalize(v2);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		VectorScreenPrintf(0, 0, resultAdd,":ADD");
		VectorScreenPrintf(0, 60, resultSubtract, ":SUBTRACT");
		VectorScreenPrintf(0, 120, resultMultiply, ":MULTIPLY");
		Novice::ScreenPrintf(0, 180, "Dot: %0.02f", resultDot);
		Novice::ScreenPrintf(0, 200, "Length: %0.02f", resultLength);
		VectorScreenPrintf(0, 220, resultNormalize, ":NORMALIZE");
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
