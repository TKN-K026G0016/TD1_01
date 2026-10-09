#include "stage.h"
#include "player.h"
#include "boss_enemy.h"
#include "scene_manager.h"

#include "texture.h"
#include "vector2.h"
#include "timer.h"
#include "common.h"

#include <Novice.h>
#include <math.h>
#include <algorithm>

#pragma region データ: 可動域

constexpr Vector2 movablePos[2] = { {0, 0}, {2560, 1440} };
constexpr Vector2 cameraMovablePos[2] = { {movablePos[0].x + 640, movablePos[0].y +360}, {movablePos[1].x - 640, movablePos[1].y - 360} };
constexpr float cameraMovablePosOffset = 64;

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

#pragma region 演出関係

//プレイヤー死亡後、ゲームオーバーへ向かうまでの時間
Timer goToGameoverTimer = { 120, 0 };

//boss死亡後、クリアへ向かうまでの時間
Timer goToClearTimer = { 120, 0 };


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

	Vector2 minPos = {
		 cameraMovablePos[0].x - cameraMovablePosOffset,
		 cameraMovablePos[0].y - cameraMovablePosOffset
	};

	Vector2 maxPos = {
		cameraMovablePos[1].x + cameraMovablePosOffset,
		cameraMovablePos[1].y + cameraMovablePosOffset
	};

	camera.pos.x = std::clamp(camera.pos.x, minPos.x, maxPos.x);
	camera.pos.y = std::clamp(camera.pos.y, minPos.y, maxPos.y);

	//if (camera.pos.x < minPos.x) {
	//	camera.pos.x = minPos.x;
	//} else if (camera.pos.x > maxPos.x) {
	//	camera.pos.x = maxPos.x;
	//}
	//if (camera.pos.y < minPos.y) {
	//	camera.pos.y = minPos.y;
	//} else if (camera.pos.y > maxPos.y) {
	//	camera.pos.y = maxPos.y;
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

#pragma region 関数: 演出関係

static void InitGoSceneTimer(void) {
	goToGameoverTimer.count = 0;
	goToClearTimer.count = 0;
}

static void GoGameoverScene(void) {
	//生存していたら、スキップ
	if (GetPlayerIsAlive()) return;
	//フェードアウトし始めたらスキップ
	if (GetNowModeFade() != ModeFade::Standby) return;

	goToGameoverTimer.count++;
	if (goToGameoverTimer.count >= goToGameoverTimer.time) {
		ChangeModeFade(ModeFade::FadeOut);
		ChangeFadeTarget(FadeTarget::Gameover);
	}
}

static void GoClearScene(void) {
	//生存していたらスキップ
	if(GetBossEnemyIsAlive()) return;

	//フェードアウトし始めたらスキップ
	if (GetNowModeFade() != ModeFade::Standby) return;

	goToClearTimer.count++;
	if (goToClearTimer.count >= goToClearTimer.time) {
		ChangeModeFade(ModeFade::FadeOut);
		ChangeFadeTarget(FadeTarget::Clear);
	}
}

#pragma endregion

void InitStage(void) {
	InitCamera();
	InitBackGround();
	InitGoSceneTimer();
}

void UpdateStage(void) {
	MoveCameraPos();
	ClampCameraPos();

	GoGameoverScene();
	GoClearScene();
}

void DrawStage(void) {

	DrawBackGround();

#ifdef _DEBUG
	//Vector2 playerPos = GetPlayerPos();
	//Novice::ScreenPrintf(1000, 20, "PlayerPos:(%.2f, %.2f)", playerPos.x, playerPos.y);

	//Novice::ScreenPrintf(1000, 40, "CameraPos:(%.2f, %.2f)", camera.pos.x, camera.pos.y);

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