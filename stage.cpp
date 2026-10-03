#include "stage.h"
#include "player.h"

#include "vector2.h"

#include <Novice.h>
#include <math.h>

#pragma region データ: カメラ関係
struct Camera {
	Vector2 pos = { 0, 0 };

	float zoom = 1.0f;
	float zoomMax = 1.0f;
	float zoomMin = 0.5f;

	//<移動処理関係>
	float moveSpeed = 5.0f;
	Vector2 moveVec = { 0, 0 };
	//補間速度
	float chaseLerpRate = 0.075f;

};
Camera camera;

#pragma endregion

#pragma region 関数: カメラ関係

static void InitCamera(void) {
	camera.pos = GetPlayerPos();
}

static void MoveCameraPos(void) {
	//追跡処理
	Vector2 targetPos = GetPlayerPos();

	//Vector2 moveVec;

	//moveVec.x = playerPos.x - camera.pos.x;
	//moveVec.y = playerPos.y - camera.pos.y;
	//float moveVecLength = sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
	//if (moveVecLength != 0) {
	//	moveVec.x /= moveVecLength;
	//	moveVec.y /= moveVecLength;
	//}

	//camera.moveVec.x = camera.moveVec.x * (1 - camera.lerpRate) + moveVec.x * camera.lerpRate;
	//camera.moveVec.y = camera.moveVec.y * (1 - camera.lerpRate) + moveVec.y * camera.lerpRate;

	//camera.pos.x += camera.moveVec.x * camera.moveSpeed;
	//camera.pos.y += camera.moveVec.y * camera.moveSpeed;

	camera.pos.x = camera.pos.x * (1 - camera.chaseLerpRate) + targetPos.x * camera.chaseLerpRate;
	camera.pos.y = camera.pos.y * (1 - camera.chaseLerpRate) + targetPos.y * camera.chaseLerpRate;
}

#pragma endregion

void InitStage(void) {
	InitCamera();
}

void UpdateStage(void) {
	MoveCameraPos();
}

void DrawStage(void) {

#ifdef _DEBUG
	Vector2 playerPos = GetPlayerPos();
	Novice::ScreenPrintf(20, 20, "PlayerPos:(%.2f, %.2f)", playerPos.x, playerPos.y);

	Novice::ScreenPrintf(20, 40, "CameraPos:(%.2f, %.2f)", camera.pos.x, camera.pos.y);

#endif // _DEBUG

}

Vector2 GetCameraPos(void) {
	return camera.pos;
}

float GetCameraZoom(void) {
	return camera.zoom;
}