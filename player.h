#pragma once
#include "vector2.h"

void InitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

/// <summary>
/// playerの座標取得
/// </summary>
Vector2 GetPlayerPos(void);