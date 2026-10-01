#pragma once
#include "vector2.h"

void InitStage(void);
void UpdateStage(void);
void DrawStage(void);

/// <summary>
/// カメラの座標取得
/// </summary>
/// <param name=""></param>
/// <returns></returns>
Vector2 GetCameraPos(void);

/// <summary>
/// カメラの拡大率取得
/// </summary>
/// <param name=""></param>
/// <returns></returns>
float GetCameraZoom(void);