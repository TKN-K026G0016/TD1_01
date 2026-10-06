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
int GetBossEnemyRemainLife(void) {
	return bossEnemy.remainLife;
}
int GetBossEnemyRemainLifeMax(void) {
	return bossEnemy.remainLifeMax;
}

void StatesCount(void) {
	if (bossEnemy.nowsSates == BossStates::Nomal) {
		stateTimer.nomal.count++;
		if (stateTimer.nomal.count == stateTimer.nomal.time) {
			stateTimer.nomal.count = 0;
			bossEnemy.nowsSates = ToInt(GetRand(1,2));
		}
	}
	else if (bossEnemy.nowsSates == BossStates::Attack1) {
		stateTimer.attack1.count++;
		if (stateTimer.attack1.count == stateTimer.attack1.time) {
			stateTimer.attack1.count = 0;
			bossEnemy.nowsSates = BossStates::Nomal;
		}
	}
	else if (bossEnemy.nowsSates == BossStates::Attack2) {
		stateTimer.attack2.count++;
		if (stateTimer.attack2.count == stateTimer.attack2.time) {
			stateTimer.attack2.count = 0;
			bossEnemy.nowsSates = BossStates::Nomal;
		}
	}
}


void MoveNomal(void) {
	if (bossEnemy.nowsSates == BossStates::Nomal) {
		playerPos = GetPlayerPos();
		float DiffX = bossEnemy.pos.x - playerPos.x;
		float DiffY = bossEnemy.pos.y - playerPos.y;
		float distance = sqrtf(DiffX * DiffX + DiffY * DiffY);
		if (distance < bossEnemy.viewRange) {
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
		else {
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
			/*bossEnemy.nowSpeed -= bossEnemy.accleretionSpeed;
			bossEnemy.nowSpeed -= bossEnemy.accleretionSpeed;
			if (bossEnemy.nowSpeed < 0) {
				bossEnemy.nowSpeed = 0;
			}*/
		}
		bossEnemy.pos.x += bossEnemy.velocity.x * bossEnemy.nowSpeed;
		bossEnemy.pos.y += bossEnemy.velocity.y * bossEnemy.nowSpeed;
	}
}

void MoveAttack1(void) {
	if (bossEnemy.nowsSates == BossStates::Attack1) {
		bossEnemy.nowsSates = BossStates::Nomal;
	}
}

void MoveAttack2(void) {
	if (bossEnemy.nowsSates == BossStates::Attack2) {
		bossEnemy.nowsSates = BossStates::Nomal;
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
		DrawTextureObj(bossEnemy.texture, bossEnemy.pos, bossEnemy.size);
	}
}

BossEnemy* GetBossEnemy(void) {
	return &bossEnemy;
}
