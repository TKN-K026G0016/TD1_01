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
	EnemySniper* enemy1 = GetEnemSniperArray();
	int enemy1Limit = GetEnemySniperLimit();

	EnemyGuardian* enemy2 = GetEnemyGuardianArray();
	int enemy2Limit = GetEnemyGuardianLimit();

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
	EnemySniper* enemy1 = GetEnemSniperArray();
	int enemy1Limit = GetEnemySniperLimit();

	EnemyGuardian* enemy2 = GetEnemyGuardianArray();
	int enemy2Limit = GetEnemyGuardianLimit();

	EnemyBasic* enemyBasic = GetEnemyBasicArray();
	int enemyBasicLimit = GetEnemyBasicLimit();

	EnemySmall* enemySmall = GetEnemySmallArray();
	int enemySmallLimit = GetEnemySmallLimit();

	EnemyM* enemyM = GetEnemyMArray();
	int enemyMLimit = GetEnemyMLimit();

	EnemyBomb* enemyBomb = GetEnemyBombArray();
	int enemyBombLimit = GetEnemyBombLimit();

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
				SpawnElecShock(enemy1[i].pos, elecBullet[j].type);

				DamageEnemySniper(enemy1[i], elecBullet[j].pow);
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
				SpawnElecShock(enemy2[i].pos, elecBullet[j].type);

				DamageEnemyGuardian(enemy2[i], elecBullet[j].pow);
			}
		}
	}


#pragma endregion

#pragma region VSEnemyBasic
	for (int i = 0; i < enemyBasicLimit; i++) {
		if (!enemyBasic[i].isAlive) continue;

		for (int j = 0; j < elecBulletLimit; j++) {
			if (!elecBullet[j].isShoot) continue;

			if (CheckCollisionCircleVSCircle(enemyBasic[i].pos, enemyBasic[i].hitRadius, elecBullet[j].pos, elecBullet[j].hitRadius)) {
				BreakElecBullet(j);
				SpawnElecShock(enemyBasic[i].pos, elecBullet[j].type);

				DamageEnemyBasic(enemyBasic[i], elecBullet[j].pow);
			}
		}
	}


#pragma endregion

#pragma region VSEnemySmall
	for (int i = 0; i < enemySmallLimit; i++) {
		if (!enemySmall[i].isAlive) continue;

		for (int j = 0; j < elecBulletLimit; j++) {
			if (!elecBullet[j].isShoot) continue;

			if (CheckCollisionCircleVSCircle(enemySmall[i].pos, enemySmall[i].hitRadius, elecBullet[j].pos, elecBullet[j].hitRadius)) {
				BreakElecBullet(j);
				//ホウデンの生成
				SpawnElecShock(enemySmall[i].pos, elecBullet[j].type);

				DamageEnemySmall(enemySmall[i], elecBullet[j].pow);
			}
		}
	}


#pragma endregion

#pragma region VSEnemyM
	for (int i = 0; i < enemyMLimit; i++) {
		if (!enemyM[i].isAlive) continue;

		for (int j = 0; j < elecBulletLimit; j++) {
			if (!elecBullet[j].isShoot) continue;

			if (CheckCollisionCircleVSCircle(enemyM[i].pos, enemyM[i].hitRadius, elecBullet[j].pos, elecBullet[j].hitRadius)) {
				BreakElecBullet(j);
				//ホウデンの生成
				SpawnElecShock(enemyM[i].pos, elecBullet[j].type);

				DamageEnemyM(enemyM[i], elecBullet[j].pow);
			}
		}
	}


#pragma endregion

#pragma region VSenemyBomb
	for (int i = 0; i < enemyBombLimit; i++) {
		if (!enemyBomb[i].isAlive) continue;

		for (int j = 0; j < elecBulletLimit; j++) {
			if (!elecBullet[j].isShoot) continue;

			if (CheckCollisionCircleVSCircle(enemyBomb[i].pos, enemyBomb[i].hitRadius, elecBullet[j].pos, elecBullet[j].hitRadius)) {
				BreakElecBullet(j);
				//ホウデンの生成
				SpawnElecShock(enemyBomb[i].pos, elecBullet[j].type);

				DamageEnemyBomb(enemyBomb[i], elecBullet[j].pow);
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
			SpawnElecShock(elecBullet[i].pos, elecBullet[i].type);

			int pow = elecBullet[i].pow;

			boss->remainLife -= pow /*- 1*/;
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
	EnemySniper* enemy1 = GetEnemSniperArray();
	int enemy1Limit = GetEnemySniperLimit();

	EnemyGuardian* enemy2 = GetEnemyGuardianArray();
	int enemy2Limit = GetEnemyGuardianLimit();

	EnemyBasic* enemyBasic = GetEnemyBasicArray();
	int enemyBasicLimit = GetEnemyBasicLimit();

	EnemySmall* enemySmall = GetEnemySmallArray();
	int enemySmallLimit = GetEnemySmallLimit();

	EnemyM* enemyM = GetEnemyMArray();
	int enemyMLimit = GetEnemyMLimit();

	EnemyBomb* enemyBomb = GetEnemyBombArray();
	int enemyBombLimit = GetEnemyBombLimit();

	ElecShock* elecShock = GetElecShockArray();
	int elecShockLimit = GetElecShockLimit();

#pragma region VSEnemy1
	for (int i = 0; i < enemy1Limit; i++) {
		if (!enemy1[i].isAlive) continue;

		for (int j = 0; j < elecShockLimit; j++) {
			if (!elecShock[j].isShoot) continue;

			if (CheckCollisionCircleVSCircle(enemy1[i].pos, enemy1[i].hitRadius, elecShock[j].pos, elecShock[j].hitRadius)) {

				//ホウデンの生成
				if (!enemy1[i].isIgnoreShock) {
					SpawnElecShock(enemy1[i].pos, elecShock[j].type);
					enemy1[i].isIgnoreShock = true;
				}
				//ダメージ判定()
				if (elecShock[j].breakTimer.count == elecShock[j].triggerDamageFrame) {
					DamageEnemySniper(enemy1[i], elecShock[j].pow);

					//コンボ
					AddShockCombo();
				}
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
				if (!enemy2[i].isIgnoreShock) {
					SpawnElecShock(enemy2[i].pos, elecShock[j].type);
					enemy2[i].isIgnoreShock = true;
				}
				//ダメージ判定()
				if (elecShock[j].breakTimer.count == elecShock[j].triggerDamageFrame) {
					DamageEnemyGuardian(enemy2[i], elecShock[j].pow);


					//コンボ
					AddShockCombo();
				}
			}
		}
	}


#pragma endregion

#pragma region VSEnemyBasic
	for (int i = 0; i < enemyBasicLimit; i++) {
		if (!enemyBasic[i].isAlive) continue;

		for (int j = 0; j < elecShockLimit; j++) {
			if (!elecShock[j].isShoot) continue;

			//無視状態時はダメージを受けない(仮)
			if (enemyBasic[i].isIgnoreShock) continue;

			if (CheckCollisionCircleVSCircle(enemyBasic[i].pos, enemyBasic[i].hitRadius, elecShock[j].pos, elecShock[j].hitRadius)) {
				//ホウデンの生成
				if (!enemyBasic[i].isIgnoreShock) {
					SpawnElecShock(enemyBasic[i].pos, elecShock[j].type);
					enemyBasic[i].isIgnoreShock = true;
				}
				//ダメージ判定()
				if (elecShock[j].breakTimer.count == elecShock[j].triggerDamageFrame) {
					DamageEnemyBasic(enemyBasic[i], elecShock[j].pow);


					//コンボ
					AddShockCombo();
				}
			}
		}
	}


#pragma endregion

#pragma region VSEnemySmall
	for (int i = 0; i < enemySmallLimit; i++) {
		if (!enemySmall[i].isAlive) continue;

		for (int j = 0; j < elecShockLimit; j++) {
			if (!elecShock[j].isShoot) continue;

			if (CheckCollisionCircleVSCircle(enemySmall[i].pos, enemySmall[i].hitRadius, elecShock[j].pos, elecShock[j].hitRadius)) {

				//ホウデンの生成
				if (!enemySmall[i].isIgnoreShock) {
					SpawnElecShock(enemySmall[i].pos, elecShock[j].type);
					enemySmall[i].isIgnoreShock = true;
				}
				//ダメージ判定()
				if (elecShock[j].breakTimer.count == elecShock[j].triggerDamageFrame) {
					DamageEnemySmall(enemySmall[i], elecShock[j].pow);

					//コンボ
					AddShockCombo();
				}
			}

		}
	}


#pragma endregion

#pragma region VSEnemyM
	for (int i = 0; i < enemyMLimit; i++) {
		if (!enemyM[i].isAlive) continue;

		for (int j = 0; j < elecShockLimit; j++) {
			if (!elecShock[j].isShoot) continue;

			if (CheckCollisionCircleVSCircle(enemyM[i].pos, enemyM[i].hitRadius, elecShock[j].pos, elecShock[j].hitRadius)) {

				//ホウデンの生成
				if (!enemyM[i].isIgnoreShock) {
					SpawnElecShock(enemyM[i].pos, elecShock[j].type);
					enemyM[i].isIgnoreShock = true;
				}
				//ダメージ判定()
				if (elecShock[j].breakTimer.count == elecShock[j].triggerDamageFrame) {
					DamageEnemyM(enemyM[i], elecShock[j].pow);


					//コンボ
					AddShockCombo();
				}
			}

		}
	}


#pragma endregion

#pragma region VSEnemyM
	for (int i = 0; i < enemyBombLimit; i++) {
		if (!enemyBomb[i].isAlive) continue;

		for (int j = 0; j < elecShockLimit; j++) {
			if (!elecShock[j].isShoot) continue;

			if (CheckCollisionCircleVSCircle(enemyBomb[i].pos, enemyBomb[i].hitRadius, elecShock[j].pos, elecShock[j].hitRadius)) {

				//ホウデンの生成
				if (!enemyBomb[i].isIgnoreShock) {
					SpawnElecShock(enemyBomb[i].pos, elecShock[j].type);
					enemyBomb[i].isIgnoreShock = true;
				}
				//ダメージ判定()
				if (elecShock[j].breakTimer.count == elecShock[j].triggerDamageFrame) {
					DamageEnemyBomb(enemyBomb[i], elecShock[j].pow);


					//コンボ
					AddShockCombo();
				}
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

		if (boss->isIgnoreElecShock) continue;

		if (CheckCollisionCircleVSCircle(boss->pos, boss->hitRadius, elecShock[i].pos, elecShock[i].hitRadius)) {
			//ホウデンの生成
			if (!boss->isIgnoreElecShock) {
				SpawnElecShock(boss->pos, elecShock[i].type);
				boss->isIgnoreElecShock = true;
			}
			//ダメージ判定()
			if (elecShock[i].breakTimer.count == elecShock[i].triggerDamageFrame) {
				int pow = elecShock[i].pow;
				boss->remainLife -= pow;

				//コンボ
				AddShockCombo();
			}
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
			if (gem[j].isIgnoreElecShock) continue;

			//被弾判定
			if (CheckCollisionCircleVSCircle(elecBullet[i].pos, elecBullet[i].hitRadius, gem[j].pos, gem[j].hitRadius)) {
				SpawnElecShock(gem[j].pos, elecBullet[i].type);
				gem[j].isIgnoreElecShock = true;

				//BreakElecBullet(i);
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

			if (gem[j].isIgnoreElecShock) continue;

			//被弾判定
			if (CheckCollisionCircleVSCircle(elecShock[i].pos, elecShock[i].hitRadius, gem[j].pos, gem[j].hitRadius)) {
				SpawnElecShock(gem[j].pos, elecShock[i].type);
				gem[j].isIgnoreElecShock = true;

				//規定回数放電したら破壊
				gem[j].hp--;
				if (gem[j].hp <= 0) {
					BreakGem(j);
				}
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
			PlayerDamage(eBullet[i].pos);
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
			PlayerDamage(eLaser[i].pos);
		}

	}
}

#pragma endregion

#pragma region 関数: PlayerVS敵の判定

static void CheckCollisionPlayerVSEnemy(void) {
	if (!GetPlayerIsAlive()) return;

	EnemyBasic* enemyBasic = GetEnemyBasicArray();
	int enemyBasicLimit = GetEnemyBasicLimit();

	EnemySmall* enemySmall = GetEnemySmallArray();
	int enemySmallLimit = GetEnemySmallLimit();

	Vector2 playerPos = GetPlayerPos();
	float playerHitRadius = GetPlayerHitRadius();

#pragma region VSEnemy

	for (int i = 0; i < enemyBasicLimit; i++) {
		if (!enemyBasic[i].isAlive) continue;

		if (CheckCollisionCircleVSCircle(enemyBasic[i].pos, enemyBasic[i].hitRadius, playerPos, playerHitRadius) ){

			PlayerDamage(enemyBasic[i].pos);
		}
	}

#pragma endregion

#pragma region VSEnemySmall

	for (int i = 0; i < enemySmallLimit; i++) {
		if (!enemySmall[i].isAlive) continue;

		if (CheckCollisionCircleVSCircle(enemySmall[i].pos, enemySmall[i].hitRadius, playerPos, playerHitRadius)) {

			PlayerDamage(enemySmall[i].pos);
		}
	}

#pragma endregion


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
	CheckCollisionPlayerVSEnemy();

}