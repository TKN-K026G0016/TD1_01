#pragma once
#include "vector2.h"
#include "timer.h"
#include "texture.h"
#include "tool.h"

struct PlayerLaser {
	Vector2 pos = { 0, 0 };
	Vector2 size = { 0, 0 };

	//<移動関係>
	float moveSpeed = 20.0f;
	float rotateTheta = 0.0f;

	float disToPlayer = 0.0f;

	//射撃フラグ
	bool isShoot = false;
	//自壊タイマー
	Timer breakTimer = { 120, 0 };

	//攻撃力
	int pow = 0;

	//当たり判定の頂点座標
	Vector2 hitBoxVertex[kVertexNum] = {};

	Texture texture = {};
};

void InitPlayerLaser(void);
void UpdatePlayerLaser(void);
void DrawPlayerLaser(void);

/// <summary>
/// laserの発射処理
/// </summary>
/// <param name="pos">発射元の座標</param>
/// <param name="moveTheta">進行方向</param>
void ShootPlayerLaser(Vector2 pos, float moveTheta);

PlayerLaser* GetPlayerLaserArray(void);

int GetPlayerLaserLimit(void);

/// <summary>
/// 消滅処理
/// </summary>
/// <param name="index">番号</param>
void BreakLaser(int index);