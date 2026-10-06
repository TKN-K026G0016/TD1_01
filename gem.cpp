#include "gem.h"
#include "player.h"

#include "vector2.h"
#include "timer.h"
#include "tool.h"
#include "texture.h"

constexpr int kGemLimit = 100;

#pragma region データ

struct Gem {
	Vector2 pos = { 0, 0 };
	Vector2 size = { 32, 32 };

	bool isAlive = false;

	//当たり判定の大きさ
	float hitRadius = 35.0f;

	//エネルギー回復量
	float recoveryEnergy = 40.0f;

	Timer breakTimer = { 3000, 0 };

	Texture texture = {};
};
Gem gem[kGemLimit];

#pragma endregion

#pragma region 関数

/// <summary>
/// ジェムの消滅処理
/// </summary>
/// <param name="index"></param>
static void BreakGem(int index) {
	gem[index].isAlive = false;
	gem[index].breakTimer.count = 0;
}

static void CheckHitVSPlayer(void) {
	Vector2 playerPos = GetPlayerPos();
	float playerHitRadius = GetPlayerHitRadius();

	for (int i = 0; i < kGemLimit; i++) {
		if (!gem[i].isAlive) continue;

		//接触したら、エネルギー回復させる
		if (CheckCollisionCircleVSCircle(playerPos, playerHitRadius, gem[i].pos, gem[i].hitRadius)) {
			BreakGem(i);
			RecoveryEnergy(gem[i].recoveryEnergy);
		}
	}

}

/// <summary>
/// ジェムの自壊タイマーカウント処理
/// </summary>
/// <param name=""></param>
static void CountBreakTimer(void) {
	for (int i = 0; i < kGemLimit; i++) {
		if (!gem[i].isAlive) continue;

		gem[i].breakTimer.count++;
		if (gem[i].breakTimer.count >= gem[i].breakTimer.time) {
			BreakGem(i);
		}
	}
}

#pragma endregion

void InitGem(void) {
	for (int i = 0; i < kGemLimit; i++) {
		gem[i] = {};
		gem[i].texture = GetTexture(TextureType::Gem);
	}

	SpawnGem({500, 500});
}

void UpdateGem(void) {
	CheckHitVSPlayer();
	CountBreakTimer();
}

void DrawGem(void) {
	for (int i = 0; i < kGemLimit; i++) {
		if (!gem[i].isAlive) continue;

		DrawTextureObj(gem[i].texture, gem[i].pos, gem[i].size);
	}
}

#pragma region 関数: 外部参照関係

void SpawnGem(Vector2 pos) {
	for (int i = 0; i < kGemLimit; i++) {
		if (gem[i].isAlive) continue;

		gem[i].isAlive = true;
		gem[i].pos = pos;

		break;
	}
}

#pragma endregion