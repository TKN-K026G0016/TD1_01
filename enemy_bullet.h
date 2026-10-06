#pragma once
#include "vector2.h"
#include "timer.h"
#include "texture.h"

struct EnemyBullet {
	Vector2 pos = {};
	Vector2 size = { 32, 32 };

	float hitRadius = 10;

	//<移動関係>
	float moveSpeed = 6.0f;
	float moveTheta = 0.0f;

	bool isShoot = false;
	Timer breakTimer = { 200, 0 };

	Texture texture = {};
};

void InitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);

/// <summary>
/// enemyBulletの射撃処理
/// </summary>
/// <param name="shootPos">生成位置</param>
/// <param name="moveTheta">射撃方向</param>
void ShootEnemyBullet(Vector2 shootPos, float moveTheta);

void BreakEnemyBullet(int index);

EnemyBullet* GetEnemyBulletArray(void);

int GetEnemyBulletLimit(void);