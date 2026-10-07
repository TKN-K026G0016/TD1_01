#include "gem.h"
#include "player.h"

#include "vector2.h"
#include "timer.h"
#include "tool.h"
#include "texture.h"

constexpr int kGemLimit = 100;

struct GemSpec {
	GemType type;
	Vector2 size;
	float hitRadius;
	float recoveryEnergy;

	Texture texture = {};
};
GemSpec spec[ToInt(GemType::Count)] = {
	{GemType::S, {20,20}, 15.0f, 10.0f, {}},
	{GemType::M, {30,30}, 25.0f, 25.0f, {}},
	{GemType::L, {40,40}, 35.0f, 40.0f, {}},
};

#pragma region データ: ジェム本体

struct Gem {
	Vector2 pos = { 0, 0 };
	Vector2 size = { 32, 32 };

	bool isAlive = false;
	Timer breakTimer = { 3000, 0 };

	//種類
	GemType type = GemType::S;

	//当たり判定の大きさ
	float hitRadius = 0.0f;

	//エネルギー回復量
	float recoveryEnergy = 0.0f;

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
	spec[ToInt(GemType::S)].texture = GetTexture(TextureType::GemSmall);
	spec[ToInt(GemType::M)].texture = GetTexture(TextureType::Gem);
	spec[ToInt(GemType::L)].texture = GetTexture(TextureType::Gem);

	for (int i = 0; i < kGemLimit; i++) {
		gem[i] = {};
	}

	SpawnGem({ 500, 500 }, GemType::S);
}

void UpdateGem(void) {
	CheckHitVSPlayer();
	CountBreakTimer();
}

void DrawGem(void) {
	for (int i = 0; i < kGemLimit; i++) {
		if (!gem[i].isAlive) continue;

		DrawTextureObj(gem[i].texture, gem[i].pos, gem[i].size);

#ifdef _DEBUG
		DrawHitEllipse(gem[i].pos, gem[i].hitRadius, RED, true);
#endif // _DEBUG

	}
}

#pragma region 関数: 外部参照関係

void SpawnGem(Vector2 pos, GemType type) {
	for (int i = 0; i < kGemLimit; i++) {
		if (gem[i].isAlive) continue;

		gem[i].isAlive = true;
		gem[i].pos = pos;

		gem[i].type = type;
		gem[i].size = spec[ToInt(type)].size;
		gem[i].hitRadius = spec[ToInt(type)].hitRadius;
		gem[i].recoveryEnergy = spec[ToInt(type)].recoveryEnergy;
		gem[i].texture = spec[ToInt(type)].texture;

		break;
	}
}

#pragma endregion