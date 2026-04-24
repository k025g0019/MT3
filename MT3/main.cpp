#include <Novice.h>
#include <math/MathUtility.h>


#include "Vector&Matrix.h"
const char kWindowTitle[] = "LE1B_26";

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	Novice::Initialize(kWindowTitle, 1280, 720);

	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 translate{ 4.1f,2.6f,0.8f };
	Vector3 scale{ 1.5f,5.2f,7.3f };
	
	
	Vector3 point{ 2.3f,3.8f,1.4f };

	Matrix4x4 transformMatrix = {
		1.0f,2.0f,3.0f,4.0f,
		3.0f,1.0f,1.0f,2.0f,
		1.0f,4.0f,2.0f,3.0f,
		2.0f,2.0f,1.0f,3.0f
	};


	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		Matrix4x4 translateMatrix = MakeTranslationMatrix(translate);

		Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

		Vector3 transformed = Transform(point, transformMatrix);
		
		VectorScreenPrintf(0, 0, transformed, "transformed");
		MatrixScreenPrintf(0,10,translateMatrix, "translateMatrix");
		MatrixScreenPrintf(0, kRowHeight * 5+20, scaleMatrix, "scaleMatrix");

		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
