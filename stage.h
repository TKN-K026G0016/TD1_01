#pragma once
#include "vector2.h"

void InitStage(void);
void UpdateStage(void);
void DrawStage(void);

/// <summary>
/// 可動域の取得
/// </summary>
/// <param name="pos">可動域を格納する配列</param>
void GetMovablePos(Vector2 pos[2]);

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