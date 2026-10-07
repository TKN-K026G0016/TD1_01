#include "boss_enemy.h"
#include "player.h"
#include "player_laser.h"

#include "vector2.h"
#include "tool.h"
#include "input.h"
#include "texture.h"

#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

BossEnemy bossEnemy;

#pragma region データ: Bomb攻撃

struct BossBomb {
	Vector2 pos = { 0, 0 };
	Vector2 size = { 64, 64 };
	//<生存関係>
	bool isAlive = false;
	//当たり判定の大きさ
	float hitRadius = 32.0f;

	//<爆発関係>
	//爆発までの時間
	Timer burst = { 0,0 };
	//爆発範囲
	float burstRadius = 128.0f;
	Vector2 bustSize = { 256,256 };
	//<移動関係>
	//速度
	Vector2 velocity = { 0.0f, 0.0f };
	//スピード
	float nowSpeed = 0.0f;
	//加速度
	float accleretionSpeed = 0.05f;
	//スピード上限
	float moveSpeedLimit = 6.0f;
	//減速度
	float decelerationSpeed = 0.1f;
	Texture texture = {};
	Texture burstTexture = {};
};
BossBomb bossBomb[6]{};

struct StatesTimer {
	Timer nomal = { 300,0 };
	Timer attack1 = { 120,0 };
	Timer attack2 = { 180,0 };
};
StatesTimer stateTimer;

#pragma endregion

#pragma region データ: レーザー攻撃

enum class ModeShootLaser {
	//テレポート状態
	Teleport,
	//射撃準備段階
	ReadyShoot,
	//射撃状態
	Shoot,
	ShootAndRotate,
};
//レーザー攻撃の形態
ModeShootLaser nowModeShootLaser = ModeShootLaser::Teleport;

struct DataShootLaser {
	//テレポート準備にかかる時間
	Timer readyTeleportTimer = { 60, 0 };
	//テレポート終了から射撃開始までの時間
	Timer readyShootTimer = { 100, 0 };

	//射撃時間
	Timer shootTimer = { 300, 0 };
	//回転が始まるまでの時間
	int startRotateTime = 60;

	//現在の回転速度
	float nowRotateSpeed = 0.0f;
	//回転速度の加速度
	float rotateAccelerationSpeed = 0.01f;
	//回転速度の上限
	float rotateSpeedMax = 0.1f;
};
DataShootLaser dataShootLaser;

#pragma endregion

Vector2 playerPos;


void StatesCount(void) {
	if (bossEnemy.nowsSates == BossStates::Normal) {
		stateTimer.nomal.count++;
		if (stateTimer.nomal.count == stateTimer.nomal.time) {
			stateTimer.nomal.count = 0;
			bossEnemy.nowsSates = ToInt(GetRand(1, 2));
		}
	}
	else if (bossEnemy.nowsSates == BossStates::Attack1) {
		stateTimer.attack1.count++;
		if (stateTimer.attack1.count == stateTimer.attack1.time) {
			stateTimer.attack1.count = 0;
			bossEnemy.nowsSates = BossStates::Normal;
		}
	}
	else if (bossEnemy.nowsSates == BossStates::Attack2) {
		stateTimer.attack2.count++;
		if (stateTimer.attack2.count == stateTimer.attack2.time) {
			stateTimer.attack2.count = 0;
			bossEnemy.nowsSates = BossStates::Normal;
		}
	}
}

void MoveNormal(void) {
	if (bossEnemy.nowsSates == BossStates::Normal) {
		playerPos = GetPlayerPos();
		float DiffX = playerPos.x - bossEnemy.pos.x;
		float DiffY = playerPos.y - bossEnemy.pos.y;
		float distance = sqrtf(DiffX * DiffX + DiffY * DiffY);
		bossEnemy.rotateTheta = atan2f(DiffY, DiffX);
		if (distance < bossEnemy.viewRange) {
			if (DiffX != 0) {
				bossEnemy.velocity.x = -1 * (DiffX / distance);
			}
			if (DiffY != 0) {
				bossEnemy.velocity.y = -1 * (DiffY / distance);
			}
			bossEnemy.nowSpeed += bossEnemy.accleretionSpeed;
			if (bossEnemy.nowSpeed > bossEnemy.moveSpeedLimit) {
				bossEnemy.nowSpeed = bossEnemy.moveSpeedLimit;
			}
		}
		else {
			if (DiffX != 0) {
				bossEnemy.velocity.x = (DiffX / distance);
			}
			if (DiffY != 0) {
				bossEnemy.velocity.y = (DiffY / distance);
			}
			bossEnemy.nowSpeed += bossEnemy.accleretionSpeed;
			if (bossEnemy.nowSpeed > bossEnemy.moveSpeedLimit) {
				bossEnemy.nowSpeed = bossEnemy.moveSpeedLimit;
			}
		}
		bossEnemy.pos.x += bossEnemy.velocity.x * bossEnemy.nowSpeed;
		bossEnemy.pos.y += bossEnemy.velocity.y * bossEnemy.nowSpeed;
	}
}

void MoveAttack1(void) {
	if (bossEnemy.nowsSates == BossStates::Attack1) {
		playerPos = GetPlayerPos();
		if (stateTimer.attack1.count == 30 || stateTimer.attack1.count == 60 || stateTimer.attack1.count == 90) {
			for (int i = 0;i < 6;i++) {
				if (bossBomb[i].isAlive == false) {
					bossBomb[i].isAlive = true;
					bossBomb[i].pos = bossEnemy.pos;
					bossBomb[i].nowSpeed = ToFloat(GetRand(60, 120)) / 10;
					bossBomb[i].velocity = { ToFloat(GetRand(-10, 10)),ToFloat(GetRand(-10,10)) };
					if (bossBomb[i].velocity.x != 0) {
						bossBomb[i].velocity.x /= 10;
					}
					if (bossBomb[i].velocity.y != 0) {
						bossBomb[i].velocity.y /= 10;
					}
					bossBomb[i].burst.time = GetRand(100, 200);
					bossBomb[i].burst.count = 0;
					break;
				}
			}
		}
	}
	for (int i = 0;i < 6;i++) {
		if (bossBomb[i].isAlive == true) {
			bossBomb[i].nowSpeed -= bossBomb[i].decelerationSpeed;
			if (bossBomb[i].nowSpeed <= 0) {
				bossBomb[i].nowSpeed = 0;
			}
			bossBomb[i].pos.x += bossBomb[i].velocity.x * bossBomb[i].nowSpeed;
			bossBomb[i].pos.y += bossBomb[i].velocity.y * bossBomb[i].nowSpeed;

			bossBomb[i].burst.count++;
			if (bossBomb[i].burst.count == bossBomb[i].burst.time) {
				float DiffX = bossBomb[i].pos.x - playerPos.x;
				float DiffY = bossBomb[i].pos.y - playerPos.y;
				float distance = sqrtf(DiffX * DiffX + DiffY * DiffY);
				if (distance <= bossBomb[i].burstRadius) {
					PlayerDamage();
				}
				bossBomb[i].isAlive = false;
			}
		}
	}
}

void MoveAttack2(void) {
	if (bossEnemy.nowsSates == BossStates::Attack2) {


	}
}

void DeathBossEnemy(void) {
	if (bossEnemy.remainLife <= 0) {
		bossEnemy.isAlive = false;
	}

}

void InitBossEnemy(void) {
	bossEnemy = {};
	bossEnemy.texture = GetTexture(TextureType::BossEnemy);
	for (int i = 0;i < 6;i++) {
		bossBomb[i] = {};
		bossBomb[i].texture = GetTexture(TextureType::BossBom);
		bossBomb[i].burstTexture = GetTexture(TextureType::BossBomBurstRange);
	}

}

void UpdateBossEnemy(void) {
	StatesCount();

	MoveNormal();
	MoveAttack1();
	MoveAttack2();

	DeathBossEnemy();
};
void DrawBossEnemy(void) {
	if (bossEnemy.isAlive == true) {
		DrawTextureRotateObj(bossEnemy.texture, bossEnemy.pos, bossEnemy.size, bossEnemy.rotateTheta);
	}
	for (int i = 0;i < 6;i++) {
		if (bossBomb[i].isAlive == true) {
			DrawTextureObj(bossBomb[i].texture, bossBomb[i].pos, bossBomb[i].size);
			if (bossBomb[i].burst.count >= bossBomb[i].burst.time - 60) {
				DrawTextureObj(bossBomb[i].burstTexture, bossBomb[i].pos, bossBomb[i].bustSize);
			}
		}
	}
}

#pragma region 関数: 外部参照関係

BossEnemy* GetBossEnemy(void) {
	return &bossEnemy;
}

bool GetBossEnemyIsAlive(void) {
	return bossEnemy.isAlive;
}
int GetBossEnemyRemainLife(void) {
	return bossEnemy.remainLife;
}
int GetBossEnemyRemainLifeMax(void) {
	return bossEnemy.remainLifeMax;
}

#pragma endregion
