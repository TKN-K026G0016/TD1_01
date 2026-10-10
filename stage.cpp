#include "stage.h"
#include "player.h"
#include "enemy.h"
#include "boss_enemy.h"
#include "scene_manager.h"

#include "texture.h"
#include "vector2.h"
#include "timer.h"
#include "common.h"
#include "tool.h"

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

#pragma region データ: ラウンド関係

enum class Round {
	Round1,
	Round2,
	Round3,
	Round4,

	RoundBoss,

	Count
};
//現在のラウンド
Round nowRound = Round::Round1;

//経過時間
TimeData gameTime = { 0, 0, 0 };
//ラウンドの変更時間
TimeData changeRoundTime[ToInt(Round::Count)] = {
	{ 0,0,0 },
	{ 0,20,0 },
	{ 0,50,0 },
	{ 1,20,0 },
	
	//ボス
	{ 2,0,0 },
};

#pragma endregion

#pragma region データ: Enemy生成関係


//カメラ端からの生成位置のずれ
float spawnPosOffset = 100;

Timer spawnEnemyTimer = { 100, 0 };


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

#pragma region 関数: ラウンド関係

static void InitRound(void) {
	ResetTimeData(gameTime);
}


/// <summary>
/// ラウンドの変更処理
/// </summary>
static void ChangeRound(void) {
	//経過時間の更新
	CountTimeData(gameTime);

#pragma region ラウンド変更処理

	int nextRound = ToInt(nowRound) + 1;
	if (nextRound == ToInt(Round::Count)) return;

	//現在の時間の合計
	int totalGameTime = ConvertTimeDataToInt(gameTime);
	int totalNextRoundTime = ConvertTimeDataToInt(changeRoundTime[nextRound]);

	//変更時間を変えたら
	if (totalGameTime >= totalNextRoundTime) {
		nowRound = static_cast<Round>(nextRound);
	}

#pragma endregion

}

//カメラの外側周りのランダム位置を作成
Vector2 CreatePosOutCameraZone(void) {
	float camLeft = camera.pos.x - kWindowCenter.x;
	float camRight = camera.pos.x + kWindowCenter.x;
	float camTop = camera.pos.y + kWindowCenter.y;
	float camBottom = camera.pos.y - kWindowCenter.y;

	int camLeftI = ToInt(camLeft);
	int camRightI = ToInt(camRight);
	int camTopI = ToInt(camTop);
	int camBottomI = ToInt(camBottom);

	Vector2 resultPos = {};

	//どの外周ゾーンに出すか
	int zone = GetRand(0, 3);

	switch (zone) {
		//左端
	case 0:
		resultPos.x = camLeft - spawnPosOffset;
		resultPos.y = ToFloat(GetRand(camBottomI, camTopI));

		break;

		//右端
	case 1:
		resultPos.x = camRight + spawnPosOffset;
		resultPos.y = ToFloat(GetRand(camBottomI, camTopI));

		break;

		//上端
	case 2:
		resultPos.x = ToFloat(GetRand(camLeftI, camRightI));
		resultPos.y = camTop + spawnPosOffset;

		break;

		//下端
	case 3:
		resultPos.x = ToFloat(GetRand(camLeftI, camRightI));
		resultPos.y = camBottom - spawnPosOffset;

		break;
	}

	return resultPos;
}

/// <summary>
/// 敵の生成処理
/// </summary>
/// <param name=""></param>
static void SpawnEnemyByRound(void) {
	switch (nowRound) {
	case Round::Round1: {

#pragma region Enemy1
		spawnEnemyTimer.count++;
		if (spawnEnemyTimer.count >= spawnEnemyTimer.time) {
			spawnEnemyTimer.count = 0;

			//ランダムな位置を生成
			Vector2 createPos = CreatePosOutCameraZone();

			SpawnEnemy(createPos, EnemyType::Enemy1);
		}

#pragma endregion

		break;
	}
	case Round::Round2: {

		break;
	}
	case Round::Round3: {

		break;
	}
	case Round::Round4: {

		break;
	}

	case Round::RoundBoss: {


		break;
	}
	}
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

	InitRound();
}

void UpdateStage(void) {
	//カメラ関係
	MoveCameraPos();
	ClampCameraPos();

	//ラウンド関係
	ChangeRound();
	SpawnEnemyByRound();

	//演出関係
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