#pragma once
#include "vector2.h"
#include "timer.h"
#include "texture.h"

enum class GemType {
	S,
	M,
	L,

	Count
};

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

void InitGem(void);
void UpdateGem(void);
void DrawGem(void);

/// <summary>
/// ジェムの生成処理
/// </summary>
/// <param name="pos">生成座標</param>
/// <param name="pos">ジェムの大きさ</param>
void SpawnGem(Vector2 pos, GemType type);

Gem* GetGemArray(void);

int GetGemLimit(void);