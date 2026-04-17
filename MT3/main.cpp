#include <Novice.h>

#include "Vector.h"

const char kWindowTitle[] = "LE1B_26";

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	Novice::Initialize(kWindowTitle, 1280, 720);

	char keys[256] = {0};
	char preKeys[256] = {0};
	Vector3 v1 = {1.0f, 3.0f, -5.0f};
	Vector3 v2 = {4.0f, -1.0f, 2.0f};
	float k = 4.0f;

	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		Vector3 resultAdd = Add(v1, v2);
		Vector3 resultSubtract = Subtract(v1, v2);
		Vector3 resultMultiply = Multiply(k, v1);
		float resultDot = Dot(v1, v2);
		float resultLength = Length(v1);
		Vector3 resultNormalize = Normalize(v2);

		VectorScreenPrintf(0, 0, resultAdd, ":ADD");
		VectorScreenPrintf(0, 60, resultSubtract, ":SUBTRACT");
		VectorScreenPrintf(0, 120, resultMultiply, ":MULTIPLY");
		Novice::ScreenPrintf(0, 180, "Dot: %0.02f", resultDot);
		Novice::ScreenPrintf(0, 200, "Length: %0.02f", resultLength);
		VectorScreenPrintf(0, 220, resultNormalize, ":NORMALIZE");

		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
