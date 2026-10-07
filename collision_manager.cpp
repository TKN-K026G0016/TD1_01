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

	Enemy2* enemy2 = GetEnemy2Array();
	int enemy2Limit = GetEnemy2Limit();

	PlayerLaser* laser = GetPlayerLaserArray();
	int laserLimit = GetPlayerLaserLimit();

#pragma region VSEnemy1
	for (int i = 0; i < enemy1Limit; i++) {
		if (!enemy1[i].isAlive) continue;

		for (int j = 0; j < laserLimit; j++) {
			if (!laser[j].isShoot) continue;

			if (CheckCollisionOBBvsCircle(enemy1[i].pos, enemy1[i].hitRadius, laser[j].hitBoxVertex)) {
				//Lv2は敵を貫通
				if (laser[j].level != LaserLevel::Level2) {
					BreakLaser(j);
				}

				int pow = GetLaserPow(j);

				enemy1[i].hp -= pow;
			}
		}
	}


#pragma endregion

#pragma region VSEnemy2
	for (int i = 0; i < enemy2Limit; i++) {
		if (!enemy2[i].isAlive) continue;

		for (int j = 0; j < laserLimit; j++) {
			if (!laser[j].isShoot) continue;

			if (CheckCollisionOBBvsCircle(enemy2[i].pos, enemy2[i].hitRadius, laser[j].hitBoxVertex)) {
				//Lv2は敵を貫通
				if (laser[j].level != LaserLevel::Level2) {
					BreakLaser(j);
				}

				int pow = GetLaserPow(j);

				enemy2[i].hp -= pow;
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
	float playerDodgeCloseRadius = GetPlayerDodgeCloseRadius();

	EnemyBullet* eBullet = GetEnemyBulletArray();
	int eBulletLimit = GetEnemyBulletLimit();

	for (int i = 0; i < eBulletLimit; i++) {
		if (!eBullet[i].isShoot) continue;

		//ギリ避け判定
		if (GetDodgeCloseSwitch()) {
			if (CheckCollisionCircleVSCircle(eBullet[i].pos, eBullet[i].hitRadius, playerPos, playerDodgeCloseRadius)) {
				TriggerDodgeClose(eBullet[i].pos);
			}
		}

		//被弾判定
		if (CheckCollisionCircleVSCircle(eBullet[i].pos, eBullet[i].hitRadius, playerPos, playerHitRadius)) {
			PlayerDamage();
		}

	}
}

/// <summary>
/// PlayerとenemyBulletの判定
/// </summary>
static void CheckCollisionPlayerVSEnemyLaser(void) {
	if (!GetPlayerIsAlive()) return;
	Vector2 playerPos = GetPlayerPos();
	float playerHitRadius = GetPlayerHitRadius();
	float playerDodgeCloseRadius = GetPlayerDodgeCloseRadius();

	EnemyLaser* eLaser = GetEnemyLaserArray();
	int eLaserLimit = GetEnemyLaserLimit();

	for (int i = 0; i < eLaserLimit; i++) {
		if (!eLaser[i].isShoot) continue;

		//ギリ避け判定
		if (GetDodgeCloseSwitch()) {
			if (CheckCollisionOBBvsCircle(playerPos, playerDodgeCloseRadius, eLaser[i].hitBoxVertex)) {
				TriggerDodgeClose(eLaser[i].pos);
			}
		}

		//被弾判定
		if (CheckCollisionOBBvsCircle(playerPos, playerHitRadius, eLaser[i].hitBoxVertex)) {
			PlayerDamage();
		}

	}
}


/// <summary>
/// PlayerLaserとBossEnemyの判定
/// </summary> 
void CheckCollisionLaserVSBossEnemy(void) {
	PlayerLaser* laser = GetPlayerLaserArray();
	int laserLimit = GetPlayerLaserLimit();
	BossEnemy* boss = GetBossEnemy();
	if (!boss->isAlive) return;
	for (int i = 0; i < laserLimit; i++) {
		if (!laser[i].isShoot) continue;
		if (CheckCollisionOBBvsCircle(boss->pos, boss->hitRadius, laser[i].hitBoxVertex)) {
			BreakLaser(i);

			int pow = GetLaserPow(i);

			boss->remainLife -= pow;
		}
	}
}

void InitCollision() {

}

void UpdateCollision() {
	CheckCollisionLaserVSEnemy();
	CheckCollisionPlayerVSEnemyBullet();
	CheckCollisionLaserVSBossEnemy();
	CheckCollisionPlayerVSEnemyLaser();
}