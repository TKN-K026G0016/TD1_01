#pragma once
#include "vector2.h"

enum class GemType {
	S,
	M,
	L,

	Count
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