#pragma once
#include "vector2.h"

void InitPlayerLaser(void);
void UpdatePlayerLaser(void);
void DrawPlayerLaser(void);

/// <summary>
/// laserの発射処理
/// </summary>
/// <param name="pos">発射元の座標</param>
/// <param name="moveTheta">進行方向</param>
void ShootPlayerLaser(Vector2 pos, float moveTheta);