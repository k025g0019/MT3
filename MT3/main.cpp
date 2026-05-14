#include <Novice.h>
#include <math/MathUtility.h>


#include "Vector&Matrix.h"
constexpr char kWindowTitle[] = "LE1B_26";

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	Novice::Initialize(kWindowTitle, 1280, 720);

	char keys[256] = {0};
	char preKeys[256] = {0};
	Vector3 scale{1.2f, 0.79f, -2.1f};
	Vector3 rotate{0.4f, 1.43f, -0.8f};

	Vector3 translate{2.7f, -4.15f, 1.57f};

	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);


		Matrix4x4 wordMatrix = MakeAffineMatrix(scale, rotate, translate);
		MatrixScreenPrintf(0, 0, wordMatrix, "wordMatrix");
		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
