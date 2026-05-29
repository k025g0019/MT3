#include <Novice.h>

#ifdef USE_IMGUI
#include <imgui.h>
#endif

#include <algorithm>

#include "OrbitCamera.h"
#include "Vector&Matrix.h"
#include "bezier.h"
constexpr char kWindowTitle[] = "LE1B_26";
constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	char keys[256] = {0};
	char preKeys[256] = {0};

	//ワールド行列の初期化
	Vector3 translates[3] = {
		{0.2f, 1.0f, 0.0f},
		{0.4f, 0.0f, 0.0f},
		{0.3f, 0.0f, 0.0f}
	};

	Vector3 rotates[3] = {
		{0.0f, 0.0f, -6.08f},
		{0.0f, 0.0f, -1.4f},
		{0.0f, 0.0f, 0.0f}
	};


	Vector3 scales[3] = {
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f}
	};
	//カメラの初期化
	OrbitCamera orbitCamera{};
	InitializeOrbitCamera(orbitCamera, {0.0f, 0.0f, 0.0f}, {0.0f, 1.9f, -6.49f});


	Matrix4x4 worldMatrices[3];
	Sphere spheres[3];
	for (int i = 0; i < 3; ++i) {
		spheres[i].center = {0.0f, 0.0f, 0.0f};
		spheres[i].radius = 0.2f;
	}


	Segment segment[2] = {
		{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
		{{0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f, -1.0f}}
	};
	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);


		//========================================================
		// 必須
		//============================================================
#ifdef USE_IMGUI
		const bool canControlCamera = !ImGui::GetIO().WantCaptureMouse;
#else
		const bool canControlCamera = true;
#endif
		UpdateOrbitCamera(orbitCamera, canControlCamera);
		Vector3 cameraTranslate = GetOrbitCameraPosition(orbitCamera);
		Vector3 cameraRotate = GetOrbitCameraRotation(orbitCamera);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({1.0f, 1.0f, 1.0f}, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(
			0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(
			0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 1.0f);

		DrawGrid(viewProjectionMatrix, viewportMatrix);

		//========================================================
		// ここにコードを追加していく
		//=====================================================

		worldMatrices[0] = MakeAffineMatrix(scales[0], rotates[0], translates[0]);

		worldMatrices[1] = Multiply(
			MakeAffineMatrix(scales[1], rotates[1], translates[1]),
			worldMatrices[0]
		);

		worldMatrices[2] = Multiply(
			MakeAffineMatrix(scales[2], rotates[2], translates[2]),
			worldMatrices[1]
		);

		spheres[0].center = Transform({0.0f, 0.0f, 0.0f}, worldMatrices[0]);
		spheres[1].center = Transform({0.0f, 0.0f, 0.0f}, worldMatrices[1]);
		spheres[2].center = Transform({0.0f, 0.0f, 0.0f}, worldMatrices[2]);

		segment[0].origin = spheres[0].center;
		segment[0].diff = Subtract(spheres[1].center, spheres[0].center);

		segment[1].origin = spheres[1].center;
		segment[1].diff = Subtract(spheres[2].center, spheres[1].center);

		DrawSegment(segment[0], viewProjectionMatrix, viewportMatrix, WHITE);
		DrawSegment(segment[1], viewProjectionMatrix, viewportMatrix, WHITE);
		DrawSphere(spheres[0], viewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(spheres[1], viewProjectionMatrix, viewportMatrix, GREEN);
		DrawSphere(spheres[2], viewProjectionMatrix, viewportMatrix, BLUE);
#ifdef USE_IMGUI
		ImGui::Begin("Debug Window");

		ImGui::DragFloat3("translate[0]", &translates[0].x, 0.01f);
		ImGui::DragFloat3("translate[1]", &translates[1].x, 0.01f);
		ImGui::DragFloat3("translate[2]", &translates[2].x, 0.01f);

		ImGui::DragFloat3("rotate[0]", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("rotate[1]", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("rotate[2]", &rotates[2].x, 0.01f);

		ImGui::DragFloat3("scale[0]", &scales[0].x, 0.01f);
		ImGui::DragFloat3("scale[1]", &scales[1].x, 0.01f);
		ImGui::DragFloat3("scale[2]", &scales[2].x, 0.01f);

		ImGui::End();
#endif

		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
