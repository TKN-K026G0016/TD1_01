#include "stage.h"
#include "player.h"

#include "texture.h"
#include "vector2.h"
#include "common.h"

#include <Novice.h>
#include <math.h>

#pragma region データ: 可動域

constexpr Vector2 movablePos[2] = { {0, 0}, {2560, 1440} };

#pragma endregion

#pragma region データ: カメラ関係
struct Camera {
	Vector2 pos = { 0, 0 };

	float zoom = 1.0f;
	float zoomMax = 1.0f;
	float zoomMin = 0.5f;

	//<移動処理関係>
	float moveSpeed = 7.0f;
	float moveSpeedDece = 1.0f;
	//減速するまでの距離
	float deceleratonDis = 60.0f;
	//補間速度
	float chaseLerpRate = 0.10f;
	//デッドゾーン
	const float deadZone = 10.0f;

	Vector2 moveVec = { 0, 0 };

	//<カメラの可動域>
	Vector2 minPos = { 0 + kWindowCenter.x, 0 + kWindowCenter.y };
	Vector2 maxPos = { 2560 - kWindowCenter.x, 1440 - kWindowCenter.y };

};
Camera camera;

#pragma endregion

#pragma region データ: 背景
struct BackGround {
	Vector2 pos = { 1280, 720 };
	Vector2 size = { 2560, 1440 };

	float scrollRate = 1.0f;

	Texture texture = {};
};
BackGround backGround;

#pragma endregion

#pragma region 関数: カメラ関係

static void InitCamera(void) {
	camera.pos = GetPlayerPos();
}

static void MoveCameraPos(void) {
	//追跡処理
	Vector2 targetPos = GetPlayerPos();
	//方向計算
	Vector2 moveVec;
	moveVec.x = targetPos.x - camera.pos.x;
	moveVec.y = targetPos.y - camera.pos.y;
	float moveVecLength = sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y);

	//デッドゾーン判定
	if (moveVecLength <= camera.deadZone) {
		return;
	}

	if (moveVecLength != 0) {
		moveVec.x /= moveVecLength;
		moveVec.y /= moveVecLength;
	}
	camera.moveVec.x = camera.moveVec.x * (1 - camera.chaseLerpRate) + moveVec.x * camera.chaseLerpRate;
	camera.moveVec.y = camera.moveVec.y * (1 - camera.chaseLerpRate) + moveVec.y * camera.chaseLerpRate;

	//移動速度
	float moveSpeed = (moveVecLength > camera.deceleratonDis) ? camera.moveSpeed : camera.moveSpeedDece;

	camera.pos.x += camera.moveVec.x * moveSpeed;
	camera.pos.y += camera.moveVec.y * moveSpeed;

}

static void ClampCameraPos(void) {
	//if (camera.pos.x < camera.minPos.x) {
	//	camera.pos.x = camera.minPos.x;
	//} else if (camera.pos.x > camera.maxPos.x) {
	//	camera.pos.x = camera.maxPos.x;
	//}

	//if (camera.pos.y < camera.minPos.y) {
	//	camera.pos.y = camera.minPos.y;
	//} else if (camera.pos.y > camera.maxPos.y) {
	//	camera.pos.y = camera.maxPos.y;
	//}
}

#pragma endregion

#pragma region 関数: 背景

static void InitBackGround(void) {
	backGround = {};
	backGround.texture = GetTexture(TextureType::BackGround);
}

static void DrawBackGround(void) {
	DrawTextureBG(backGround.texture, backGround.pos, backGround.size, backGround.scrollRate);
}

#pragma endregion

void InitStage(void) {
	InitCamera();
	InitBackGround();
}

void UpdateStage(void) {
	MoveCameraPos();
	ClampCameraPos();
}

void DrawStage(void) {

	DrawBackGround();

#ifdef _DEBUG
	Vector2 playerPos = GetPlayerPos();
	Novice::ScreenPrintf(20, 20, "PlayerPos:(%.2f, %.2f)", playerPos.x, playerPos.y);

	Novice::ScreenPrintf(20, 40, "CameraPos:(%.2f, %.2f)", camera.pos.x, camera.pos.y);

#endif // _DEBUG

}

#pragma region 関数: 外部参照関係

void GetMovablePos(Vector2 pos[2]) {
	pos[0] = movablePos[0];
	pos[1] = movablePos[1];
}

Vector2 GetCameraPos(void) {
	return camera.pos;
}

float GetCameraZoom(void) {
	return camera.zoom;
}

#pragma endregion