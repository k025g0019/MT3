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

	//カメラの初期化
	OrbitCamera orbitCamera{};
	InitializeOrbitCamera(orbitCamera, {0.0f, 0.0f, 0.0f}, {0.0f, 1.9f, -6.49f});

	ConicalPendulum conicalPendulum;
	conicalPendulum.anchor = {0.0f, 1.0f, 0.0f};
	conicalPendulum.length = 0.8f;
	conicalPendulum.halfApexAngle = 0.7f;
	conicalPendulum.angle = 0.0f;
	conicalPendulum.angularVelocity = 0.0f;


	Ball ball = {};
	float deltaTime = 1.0f / 60.0f;
	Sphere sphere{

	};

	bool ispendulum = false;
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


		if (ispendulum) {
			conicalPendulum.angularVelocity = std::sqrt(
				9.8f / (conicalPendulum.length * std::cos(conicalPendulum.halfApexAngle)));
			conicalPendulum.angle += conicalPendulum.angularVelocity * deltaTime;
			float radius = conicalPendulum.length * std::sin(conicalPendulum.halfApexAngle);
			float height = conicalPendulum.length * std::cos(conicalPendulum.halfApexAngle);
			ball.position.x = conicalPendulum.anchor.x + radius * std::cos(conicalPendulum.angle);
			ball.position.y = conicalPendulum.anchor.y - height;
			ball.position.z = conicalPendulum.anchor.z - std::sin(conicalPendulum.angle) * radius;
		}
		sphere.center = ball.position;
		sphere.radius = 0.1f;

		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix,BLUE);
#ifdef USE_IMGUI
		ImGui::Begin("Debug Window");
		if (ImGui::Button("Reset Pendulum")) {
			ispendulum = !ispendulum;
		}
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
