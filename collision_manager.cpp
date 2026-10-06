#include "collision_manager.h"
#include "player_laser.h"
#include "enemy.h"
#include"boss_enemy.h"
#include "player.h"
#include "enemy_bullet.h"

#include "tool.h"
#include "vector2.h"

/// <summary>
/// playerLaserとenemyの判定
/// </summary>
static void CheckCollisionLaserVSEnemy(void) {
	Enemy1* enemy1 = GetEnemy1Array();
	int enemy1Limit = GetEnemy1Limit();

	PlayerLaser* laser = GetPlayerLaserArray();
	int laserLimit = GetPlayerLaserLimit();

#pragma region VSEnemy1
	for (int i = 0; i < enemy1Limit; i++) {
		if (!enemy1[i].isAlive) continue;

		for (int j = 0; j < laserLimit; j++) {
			if (!laser[j].isShoot) continue;

			if (CheckCollisionOBBvsCircle(enemy1[i].pos, enemy1[i].hitRadius, laser[j].hitBoxVertex)) {
				BreakLaser(j);

				enemy1[i].hp -= laser[j].pow;
			}
		}
	}


#pragma endregion

}

/// <summary>
/// PlayerとenemyBulletの判定
/// </summary>
static void CheckCollisionPlayerVSEnemyBullet(void) {
	if (!GetPlayerIsAlive()) return;
	Vector2 playerPos = GetPlayerPos();
	float playerHitRadius = GetPlayerHitRadius();

	EnemyBullet* eBullet = GetEnemyBulletArray();
	int eBulletLimit = GetEnemyBulletLimit();

	for (int i = 0; i < eBulletLimit; i++) {
		if (!eBullet[i].isShoot) continue;

		if (CheckCollisionCircleVSCircle(eBullet[i].pos, eBullet[i].hitRadius, playerPos, playerHitRadius)) {
			BreakEnemyBullet(i);

			PlayerDamage();
		}

	}
}


/// <summary>
/// PlayerLaserとBossEnemyの判定
/// </summary>
/// <param name=""></param>
/// 
void CheckCollisionLaserVSBossEnemy(void) {
	PlayerLaser* laser = GetPlayerLaserArray();
	int laserLimit = GetPlayerLaserLimit();
	BossEnemy* boss = GetBossEnemy();
	if (!boss->isAlive) return;
	for (int i = 0; i < laserLimit; i++) {
		if (!laser[i].isShoot) continue;
		if (CheckCollisionOBBvsCircle(boss->pos, boss->hitRadius, laser[i].hitBoxVertex)) {
			BreakLaser(i);
			boss->remainLife -= laser[i].pow;
		}
	}
}

void InitCollision() {

}

void UpdateCollision() {
	CheckCollisionLaserVSEnemy();
	CheckCollisionPlayerVSEnemyBullet();
	CheckCollisionLaserVSBossEnemy();
}