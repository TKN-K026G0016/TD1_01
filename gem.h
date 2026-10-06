#pragma once
#include "vector2.h"

void InitGem(void);
void UpdateGem(void);
void DrawGem(void);

/// <summary>
/// ジェムの生成処理
/// </summary>
/// <param name="pos">生成座標</param>
void SpawnGem(Vector2 pos);