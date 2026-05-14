#include <Novice.h>
#include <math/MathUtility.h>

#include  "Matrix.h"
#include "Vector&Matrix.h"
#include  "Vector.h"
constexpr char kWindowTitle[] = "LE1B_26";

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	Novice::Initialize(kWindowTitle, 1280, 720);

	char keys[256] = {0};
	char preKeys[256] = {0};


	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		Matrix4x4 orthographicMatrix = MakeOrthographicMatrix(-160.f, 160.f, 200.0f, 300.0f, 0.0f, 1000.0f);

		Matrix4x4 perspectiveFovMatrix = MakePerspectiveFovMatrix(0.63f, 1.33f, 0.1f, 1000.0f);

		Matrix4x4 viewporMatrix = MakeViewportMatrix(100.0f, 200.0f, 600.0f, 300.0f, 0.0f, 1.0f);

		MatrixScreenPrintf(0, 0, orthographicMatrix, "Orthographic Matrix");
		MatrixScreenPrintf(0, kRowHeight * 5, perspectiveFovMatrix, "Perspective Fov Matrix");
		MatrixScreenPrintf(0, kRowHeight * 10, viewporMatrix, "Viewport Matrix");
		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
