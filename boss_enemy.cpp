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

struct BossBom {
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
BossBom bossBom[6]{};

struct StatesTimer {
	Timer nomal = { 300,0 };
	Timer attack1 = { 120,0 };
	Timer attack2 = { 180,0 };
};
StatesTimer stateTimer;

Vector2 playerPos;


bool GetBossEnemyIsAlive(void) {
	return bossEnemy.isAlive;
}
Vector2 GetBossEnemyPos(void) {
	return bossEnemy.pos;
}
float GetBossEnemyRotateTheta(void) {
	return bossEnemy.rotateTheta;
}
int GetBossEnemyRemainLife(void) {
	return bossEnemy.remainLife;
}
int GetBossEnemyRemainLifeMax(void) {
	return bossEnemy.remainLifeMax;
}
int GetBossEnemyNowStates(void) {
	return bossEnemy.nowsSates;
}

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


void MoveNomal(void) {
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
				if (bossBom[i].isAlive == false) {
					bossBom[i].isAlive = true;
					bossBom[i].pos = bossEnemy.pos;
					bossBom[i].nowSpeed = ToFloat(GetRand(60, 120)) / 10;
					bossBom[i].velocity = { ToFloat(GetRand(-10, 10)),ToFloat(GetRand(-10,10)) };
					if (bossBom[i].velocity.x != 0) {
						bossBom[i].velocity.x /= 10;
					}
					if (bossBom[i].velocity.y != 0) {
						bossBom[i].velocity.y /= 10;
					}
					bossBom[i].burst.time = GetRand(100, 200);
					bossBom[i].burst.count = 0;
					break;
				}
			}
		}
	}
	for (int i = 0;i < 6;i++) {
		if (bossBom[i].isAlive == true) {
			bossBom[i].nowSpeed -= bossBom[i].decelerationSpeed;
			if (bossBom[i].nowSpeed <= 0) {
				bossBom[i].nowSpeed = 0;
			}
			bossBom[i].pos.x += bossBom[i].velocity.x * bossBom[i].nowSpeed;
			bossBom[i].pos.y += bossBom[i].velocity.y * bossBom[i].nowSpeed;

			bossBom[i].burst.count++;
			if (bossBom[i].burst.count == bossBom[i].burst.time) {
				float DiffX = bossBom[i].pos.x - playerPos.x;
				float DiffY = bossBom[i].pos.y - playerPos.y;
				float distance = sqrtf(DiffX * DiffX + DiffY * DiffY);
				if (distance <= bossBom[i].burstRadius) {
					PlayerDamage();
				}
				bossBom[i].isAlive = false;
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
		bossBom[i] = {};
		bossBom[i].texture = GetTexture(TextureType::BossBom);
		bossBom[i].burstTexture = GetTexture(TextureType::BossBomBurstRange);
	}

}

void UpdateBossEnemy(void) {
	StatesCount();

	MoveNomal();
	MoveAttack1();
	MoveAttack2();

	DeathBossEnemy();
};
void DrawBossEnemy(void) {
	if (bossEnemy.isAlive == true) {
		DrawTextureRotateObj(bossEnemy.texture, bossEnemy.pos, bossEnemy.size, bossEnemy.rotateTheta);
	}
	for (int i = 0;i < 6;i++) {
		if (bossBom[i].isAlive == true) {
			DrawTextureObj(bossBom[i].texture, bossBom[i].pos, bossBom[i].size);
			if (bossBom[i].burst.count >= bossBom[i].burst.time - 60) {
				DrawTextureObj(bossBom[i].burstTexture, bossBom[i].pos, bossBom[i].bustSize);
			}
		}
	}
}

BossEnemy* GetBossEnemy(void) {
	return &bossEnemy;
}
