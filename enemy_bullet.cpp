#include "enemy_bullet.h"

#include "vector2.h"
#include "timer.h"
#include "tool.h"
#include "texture.h"

#include <math.h>

static constexpr int kEnemyBulletLimit = 20;

static EnemyBullet enemyBullet[kEnemyBulletLimit];

#pragma region 関数

static void MoveEnemyBullet(void) {
	for (int i = 0; i < kEnemyBulletLimit; i++) {
		if (!enemyBullet[i].isShoot) continue;

		enemyBullet[i].pos.x += cosf(enemyBullet[i].moveTheta) * enemyBullet[i].moveSpeed;
		enemyBullet[i].pos.y += sinf(enemyBullet[i].moveTheta) * enemyBullet[i].moveSpeed;
	}
}

/// <summary>
/// 自壊タイマーのカウント処理
/// </summary>
/// <param name=""></param>
static void BreakEnemyBulletItSelf(void) {
	for (int i = 0; i < kEnemyBulletLimit; i++) {
		if (!enemyBullet[i].isShoot) {
			continue;
		}

		enemyBullet[i].breakTimer.count++;
		if (enemyBullet[i].breakTimer.count >= enemyBullet[i].breakTimer.time) {
			BreakEnemyBullet(i);
		}
	}
}

#pragma endregion

void InitEnemyBullet(void) {
	for (int i = 0; i < kEnemyBulletLimit; i++) {
		enemyBullet[i] = {};

		enemyBullet[i].texture = GetTexture(TextureType::EnemyBullet);
	}
}

void UpdateEnemyBullet(void) {
	MoveEnemyBullet();
	BreakEnemyBulletItSelf();
}

void DrawEnemyBullet(void) {
	for (int i = 0; i < kEnemyBulletLimit; i++) {
		if (!enemyBullet[i].isShoot) continue;

		UpdateAnimation(enemyBullet[i].texture);
		DrawTextureObj(enemyBullet[i].texture, enemyBullet[i].pos, enemyBullet[i].size);
	}
}

#pragma region 関数: 外部参照関係

void ShootEnemyBullet(Vector2 shootPos, float moveTheta) {
	for (int i = 0; i < kEnemyBulletLimit; i++) {
		if (enemyBullet[i].isShoot) continue;

		enemyBullet[i].isShoot = true;
		enemyBullet[i].pos = shootPos;
		enemyBullet[i].moveTheta = moveTheta;

		break;
	}
}

EnemyBullet* GetEnemyBulletArray(void) {
	return enemyBullet;
}

int GetEnemyBulletLimit(void) {
	return kEnemyBulletLimit;
}

void BreakEnemyBullet(int index) {
	enemyBullet[index].isShoot = false;
	enemyBullet[index].breakTimer.count = 0;
}

#pragma endregion