#include "collision_manager.h"
#include "player_laser.h"
#include "elec_bullet.h"
#include "enemy.h"
#include"boss_enemy.h"
#include "player.h"
#include "enemy_bullet.h"
#include "gem.h"

#include "tool.h"
#include "vector2.h"


#pragma region 関数: playerLaserVS敵の判定
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

			boss->remainLife -= pow - 1;
		}
	}
}

#pragma endregion

#pragma region 関数: elecBulletVS敵の当たり判定

/// <summary>
/// ElecBulletとenemyの判定
/// </summary>
/// <param name=""></param>
static void CheckCollisionElecBulletVSEnemy(void) {
	Enemy1* enemy1 = GetEnemy1Array();
	int enemy1Limit = GetEnemy1Limit();

	Enemy2* enemy2 = GetEnemy2Array();
	int enemy2Limit = GetEnemy2Limit();

	ElecBullet* elecBullet = GetElecBulletArray();
	int elecBulletLimit = GetElecBulletLimit();

#pragma region VSEnemy1
	for (int i = 0; i < enemy1Limit; i++) {
		if (!enemy1[i].isAlive) continue;

		for (int j = 0; j < elecBulletLimit; j++) {
			if (!elecBullet[j].isShoot) continue;

			if (CheckCollisionCircleVSCircle(enemy1[i].pos, enemy1[i].hitRadius, elecBullet[j].pos, elecBullet[j].hitRadius)) {
				BreakElecBullet(j);
				//ホウデンの生成
				SpawnElecShock(enemy1[i].pos);


				int pow = elecBullet[j].pow;

				enemy1[i].hp -= pow;
			}
		}
	}


#pragma endregion

#pragma region VSEnemy2
	for (int i = 0; i < enemy2Limit; i++) {
		if (!enemy2[i].isAlive) continue;

		for (int j = 0; j < elecBulletLimit; j++) {
			if (!elecBullet[j].isShoot) continue;

			if (CheckCollisionCircleVSCircle(enemy2[i].pos, enemy2[i].hitRadius, elecBullet[j].pos, elecBullet[j].hitRadius)) {
				BreakElecBullet(j);
				SpawnElecShock(enemy2[i].pos);


				int pow = elecBullet[j].pow;

				enemy2[i].hp -= pow;
			}
		}
	}


#pragma endregion
}

/// <summary>
/// ElecBulletとBossEnemyの判定
/// </summary> 
void CheckCollisionElecBulletVSBossEnemy(void) {
	ElecBullet* elecBullet = GetElecBulletArray();
	int elecBulletLimit = GetElecBulletLimit();
	BossEnemy* boss = GetBossEnemy();
	if (!boss->isAlive) return;
	for (int i = 0; i < elecBulletLimit; i++) {
		if (!elecBullet[i].isShoot) continue;
		if (CheckCollisionCircleVSCircle(boss->pos, boss->hitRadius, elecBullet[i].pos, elecBullet[i].hitRadius)) {
			BreakElecBullet(i);
			//ホウデンの生成
			SpawnElecShock(elecBullet[i].pos);

			int pow = elecBullet[i].pow;

			boss->remainLife -= pow - 1;
		}
	}
}

#pragma endregion

#pragma region 関数: elecShockVS敵の当たり判定

/// <summary>
/// ElecShockとenemyの判定
/// </summary>
/// <param name=""></param>
static void CheckCollisionElecShockVSEnemy(void) {
	Enemy1* enemy1 = GetEnemy1Array();
	int enemy1Limit = GetEnemy1Limit();

	Enemy2* enemy2 = GetEnemy2Array();
	int enemy2Limit = GetEnemy2Limit();

	ElecShock* elecShock = GetElecShockArray();
	int elecShockLimit = GetElecShockLimit();

#pragma region VSEnemy1
	for (int i = 0; i < enemy1Limit; i++) {
		if (!enemy1[i].isAlive) continue;

		for (int j = 0; j < elecShockLimit; j++) {
			if (!elecShock[j].isShoot) continue;

			//無視状態時はダメージを受けない(仮)
			if (enemy1[i].isIgnoreShock) continue;

			if (CheckCollisionCircleVSCircle(enemy1[i].pos, enemy1[i].hitRadius, elecShock[j].pos, elecShock[j].hitRadius)) {
				//ホウデンの生成
				SpawnElecShock(enemy1[i].pos);
				enemy1[i].isIgnoreShock = true;

				int pow = elecShock[j].pow;
				enemy1[i].hp -= pow;
			}

		}
	}


#pragma endregion

#pragma region VSEnemy2
	for (int i = 0; i < enemy2Limit; i++) {
		if (!enemy2[i].isAlive) continue;

		for (int j = 0; j < elecShockLimit; j++) {
			if (!elecShock[j].isShoot) continue;

			//無視状態時はダメージを受けない(仮)
			if (enemy2[i].isIgnoreShock) continue;

			if (CheckCollisionCircleVSCircle(enemy2[i].pos, enemy2[i].hitRadius, elecShock[j].pos, elecShock[j].hitRadius)) {
				//ホウデンの生成
				SpawnElecShock(enemy2[i].pos);
				enemy2[i].isIgnoreShock = true;

				int pow = elecShock[j].pow;
				enemy2[i].hp -= pow;
			}
		}
	}


#pragma endregion
}

/// <summary>
/// ElecShockとBossEnemyの判定
/// </summary> 
void CheckCollisionElecShockVSBossEnemy(void) {
	ElecShock* elecShock = GetElecShockArray();
	int elecShockLimit = GetElecShockLimit();
	BossEnemy* boss = GetBossEnemy();
	if (!boss->isAlive) return;
	for (int i = 0; i < elecShockLimit; i++) {
		if (!elecShock[i].isShoot) continue;
		if (CheckCollisionCircleVSCircle(boss->pos, boss->hitRadius, elecShock[i].pos, elecShock[i].hitRadius)) {
			BreakElecShock(i);
			//ホウデンの生成
			SpawnElecShock(boss->pos);

			int pow = elecShock[i].pow;

			boss->remainLife -= pow - 1;
		}
	}
}

#pragma endregion

#pragma region 関数: elecBulletVSジェムの判定

static void CheckCollisionElecBulletVSGem(void) {
	Gem* gem = GetGemArray();
	int gemLimit = GetGemLimit();


	ElecBullet* elecBullet = GetElecBulletArray();
	int elecBulletLimit = GetElecBulletLimit();

	for (int i = 0; i < elecBulletLimit; i++) {
		if (!elecBullet[i].isShoot) continue;

		for (int j = 0; j < gemLimit; j++) {
			if (!gem[j].isAlive) continue;

			//被弾判定
			if (CheckCollisionCircleVSCircle(elecBullet[i].pos, elecBullet[i].hitRadius, gem[i].pos, gem[i].hitRadius)) {
				SpawnElecShock(gem[j].pos);
			}
		}

	}
}

static void CheckCollisionElecShockVSGem(void) {
	Gem* gem = GetGemArray();
	int gemLimit = GetGemLimit();


	ElecShock* elecShock = GetElecShockArray();
	int elecShockLimit = GetElecShockLimit();

	for (int i = 0; i < elecShockLimit; i++) {
		if (!elecShock[i].isShoot) continue;

		for (int j = 0; j < gemLimit; j++) {
			if (!gem[j].isAlive) continue;

			//被弾判定
			if (CheckCollisionCircleVSCircle(elecShock[i].pos, elecShock[i].hitRadius, gem[i].pos, gem[i].hitRadius)) {
				SpawnElecShock(gem[j].pos);
			}
		}

	}
}

#pragma endregion

#pragma region 関数: PlayerVS敵弾の判定
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

#pragma endregion

void InitCollision() {

}

void UpdateCollision() {
	CheckCollisionLaserVSEnemy();
	CheckCollisionLaserVSBossEnemy();

	CheckCollisionElecBulletVSEnemy();
	CheckCollisionElecBulletVSBossEnemy();

	CheckCollisionElecShockVSEnemy();
	CheckCollisionElecShockVSBossEnemy();

	CheckCollisionElecBulletVSGem();
	CheckCollisionElecShockVSGem();

	CheckCollisionPlayerVSEnemyBullet();
	CheckCollisionPlayerVSEnemyLaser();

}