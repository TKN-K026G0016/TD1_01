#include "enemy.h"
#include "player.h"
#include "stage.h"
#include "enemy_bullet.h"
#include "gem.h"
#include "boss_enemy.h"

#include "vector2.h"
#include "timer.h"
#include "tool.h"
#include "texture.h"

#define _USE_MATH_DEFINES
#include <math.h>

//カラー関係
static constexpr unsigned int kNormalColor = 0xE01068FF;
static constexpr unsigned int kFlashColor = 0x333333FF;

#pragma region データ: Enemy1
constexpr int kEnemy1Limit = 10;
static Enemy1 enemy1[kEnemy1Limit];
int nowEnemy1IsAliveCount = 0;
//ジェムの生成量
constexpr int enemy1GemLimit = 4;

#pragma endregion

#pragma region データ: Enemy2
constexpr int kEnemy2Limit = 10;
static Enemy2 enemy2[kEnemy2Limit];

#pragma endregion

#pragma region データ: EnemyBasic
constexpr int kEnemyBasicLimit = 20;
static EnemyBasic enemyBasic[kEnemyBasicLimit];
//ジェムの生成量
constexpr int enemyBasicGemLimit = 3;

#pragma endregion

#pragma region データ: EnemyM
constexpr int kEnemyMLimit = 4;
static EnemyM enemyM[kEnemyMLimit];
//ジェムの生成量
constexpr int enemyMGemLimit = 4;

#pragma endregion

#pragma region データ: EnemySmall
constexpr int kEnemySmallLimit = 30;
static EnemySmall enemySmall[kEnemySmallLimit];
//ジェムの生成量
constexpr int enemySmallGemLimit = 1;

#pragma endregion

#pragma region データ: EnemySmallGroup

//散らばる範囲(最大)
constexpr int kScatterLengthMax = 100;
//散らばる範囲(最小)
constexpr int kScatterLengthMin = 0;

#pragma endregion

#pragma region データ: EnemyBomb
constexpr int kEnemyBombLimit = 10;
static EnemyBomb enemyBomb[kEnemyBombLimit];
//ジェムの生成量
constexpr int enemyBombGemLimit = 2;

#pragma endregion





struct Spawner {
	Timer spawnTimer = { 0, 0 };

	Vector2 spawnRange[2] = {};

	int kSpawnTime = 20;
};
Spawner spawner;



#pragma region 関数: enemy1

static void InitEnemy1(void) {
	for (int i = 0; i < kEnemy1Limit; i++) {
		enemy1[i] = {};

		enemy1[i].texture = GetTexture(TextureType::Enemy1);
	}
}

static void ShootEnemyBullet1(void) {
	for (int i = 0; i < kEnemy1Limit; i++) {
		if (!enemy1[i].isAlive) continue;

		enemy1[i].shootTimer.count++;
		if (enemy1[i].shootTimer.count >= enemy1[i].shootTimer.time) {
			enemy1[i].shootTimer.count = 0;

			Vector2 playerPos = GetPlayerPos();

			float disX = playerPos.x - enemy1[i].pos.x;
			float disY = playerPos.y - enemy1[i].pos.y;
			float shootTheta = atan2f(disY, disX);
			ShootEnemyBullet(enemy1[i].pos, shootTheta);

		}

	}
}

static void MoveEnemy1(void) {
	Vector2 playerPos = GetPlayerPos();
	for (int i = 0;i < kEnemy1Limit;i++) {
		if (!enemy1[i].isAlive)continue;

		float diffX = playerPos.x - enemy1[i].pos.x;
		float diffY = playerPos.y - enemy1[i].pos.y;
		float distance = sqrtf(diffX * diffX + diffY * diffY);
		if (distance > enemy1[i].viewRange) {
			enemy1[i].velocity = { diffX / distance,diffY / distance };
			enemy1[i].nowSpeed += enemy1[i].accleretionSpeed;
			if (enemy1[i].nowSpeed > enemy1[i].moveSpeedLimit) {
				enemy1[i].nowSpeed = enemy1[i].moveSpeedLimit;
			}
			enemy1[i].pos.x += enemy1[i].velocity.x * enemy1[i].nowSpeed;
			enemy1[i].pos.y += enemy1[i].velocity.y * enemy1[i].nowSpeed;
		}
		else {
			enemy1[i].nowSpeed -= enemy1[i].decelerationSpeed;
			if (enemy1[i].nowSpeed < 0) {
				enemy1[i].nowSpeed = 0;
			}
			ShootEnemyBullet1();
		}
	}
}

/// <summary>
/// デンゲキショックの生成を受け付けない状態のカウント
/// </summary>
/// <param name=""></param>
static void CountIgnoreShockEnemy1(void) {
	for (int i = 0; i < kEnemy1Limit; i++) {
		if (!enemy1[i].isAlive) continue;
		if (!enemy1[i].isIgnoreShock) continue;


		enemy1[i].ignoreSpawnShockTimer.count++;
		if (enemy1[i].ignoreSpawnShockTimer.count >= enemy1[i].ignoreSpawnShockTimer.time) {
			enemy1[i].ignoreSpawnShockTimer.count = 0;
			enemy1[i].isIgnoreShock = false;
		}
	}
}

static void UpdateFlashTimerEnemy1(void) {
	for (int i = 0; i < kEnemy1Limit; i++) {
		if (!enemy1[i].isAlive) continue;
		if (!enemy1[i].isFlash) continue;

		enemy1[i].flashTimer.count++;
		if (enemy1[i].flashTimer.count >= enemy1[i].flashTimer.time) {
			enemy1[i].flashTimer.count = 0;
			enemy1[i].isFlash = false;
		}

	}
}

static void DeadEnemy1(int index) {
	enemy1[index].isAlive = false;
	enemy1[index].hp = enemy1[index].hpMax;
	nowEnemy1IsAliveCount--;
	spawner.spawnTimer.time = spawner.kSpawnTime * nowEnemy1IsAliveCount;
}

static void CheckDeadEnemy1(void) {
	for (int i = 0; i < kEnemy1Limit; i++) {
		if (!enemy1[i].isAlive) continue;

		if (enemy1[i].hp <= 0) {
			DeadEnemy1(i);

			//ジェムの生成処理
			for (int j = 0;j < enemy1GemLimit;j++) {
				SpawnGem(enemy1[i].pos, GemType::S);
			}
		}
	}
}

static void DrawEnemy1(void) {
	for (int i = 0; i < kEnemy1Limit; i++) {
		if (!enemy1[i].isAlive) continue;

		Texture texture = enemy1[i].texture;
		if (enemy1[i].isFlash) {
			texture.color = kFlashColor;
		}

		DrawTextureObj(texture, enemy1[i].pos, enemy1[i].size);
	}
}

void ReSpawnEnemy(void) {
	//spawner.spawnTimer.count++;
	//if (spawner.spawnTimer.count >= spawner.spawnTimer.time) {
	//	spawner.spawnTimer.count = 0;
	//	Vector2i spawnRange[2] = { {ToInt(spawner.spawnRange[0].x), ToInt(spawner.spawnRange[0].y) }, {ToInt(spawner.spawnRange[1].x), ToInt(spawner.spawnRange[1].y)} };

	//	Vector2 spawnPos = { ToFloat(GetRand(spawnRange[0].x, spawnRange[1].x)), ToFloat(GetRand(spawnRange[0].y, spawnRange[1].y))};
	//	SpawnEnemy(spawnPos, EnemyType::Enemy1);
	//}
}
#pragma endregion

#pragma region 関数: enemy2

static void InitEnemy2(void) {
	for (int i = 0;i < kEnemy2Limit;i++) {
		enemy2[i] = {};

		enemy2[i].texture = GetTexture(TextureType::Enemy2);
	}
}

static void ShootEnemyBullet2(void) {
	for (int i = 0; i < kEnemy2Limit; i++) {
		if (!enemy2[i].isAlive) continue;

		enemy2[i].shootTimer.count++;
		if (enemy2[i].shootTimer.count >= enemy2[i].shootTimer.time) {
			enemy2[i].shootTimer.count = 0;

			Vector2 playerPos = GetPlayerPos();

			float disX = playerPos.x - enemy2[i].pos.x;
			float disY = playerPos.y - enemy2[i].pos.y;
			float shootTheta = atan2f(disY, disX);
			ShootEnemyBullet(enemy2[i].pos, shootTheta);

		}

	}
}

/// <summary>
/// デンゲキショックの生成を受け付けない状態のカウント
/// </summary>
/// <param name=""></param>
static void CountIgnoreShockEnemy2(void) {
	for (int i = 0; i < kEnemy2Limit; i++) {
		if (!enemy2[i].isAlive) continue;
		if (!enemy2[i].isIgnoreShock) continue;


		enemy2[i].ignoreSpawnShockTimer.count++;
		if (enemy2[i].ignoreSpawnShockTimer.count >= enemy2[i].ignoreSpawnShockTimer.time) {
			enemy2[i].ignoreSpawnShockTimer.count = 0;
			enemy2[i].isIgnoreShock = false;
		}
	}
}

static void UpdateFlashTimerEnemy2(void) {
	for (int i = 0; i < kEnemy2Limit; i++) {
		if (!enemy2[i].isAlive) continue;
		if (!enemy2[i].isFlash) continue;

		enemy2[i].flashTimer.count++;
		if (enemy2[i].flashTimer.count >= enemy2[i].flashTimer.time) {
			enemy2[i].flashTimer.count = 0;
			enemy2[i].isFlash = false;
		}
	}
}

static void DeadEnemy2(int index) {
	enemy2[index].isAlive = false;
	enemy2[index].hp = enemy2[index].hpMax;
}

static void CheckDeadEnemy2(void) {
	for (int i = 0; i < kEnemy2Limit; i++) {
		if (!enemy2[i].isAlive) continue;

		if (enemy2[i].hp <= 0) {
			DeadEnemy2(i);

			//ジェムの生成処理
			SpawnGem(enemy2[i].pos, GemType::M);
		}
	}
}

static void DrawEnemy2(void) {
	for (int i = 0; i < kEnemy2Limit; i++) {
		if (!enemy2[i].isAlive) continue;

		Texture texture = enemy2[i].texture;
		if (enemy2[i].isFlash) {
			texture.color = kFlashColor;
		}

		DrawTextureObj(texture, enemy2[i].pos, enemy2[i].size);
	}
}

static void Attack2Enemy2Move(void) {
	Attack2Enemy2Pos attack2Pos;
	int bossEnemyNouStates = GetBossEnemyNowStates();
	Timer attack2Timer = GetAttack2Timer();
	Vector2 bossEnemyPos = GetBossEnemyPos();
	float bossEnemyRotateTheta = GetBossEnemyRotateTheta();
	int enemyCount = 0;
	if (bossEnemyNouStates == Attack2) {
		if (attack2Timer.count == 0) {
			for (int i = 0;i < kEnemy2Limit;i++) {
				if (enemy2[i].isAlive)continue;
				enemy2[i].isAlive = true;
				enemyCount++;
				if (enemyCount == 6)break;
			}
		}
		for (int i = 0;i < kEnemy2Limit;i++) {
			if (!enemy2[i].isAlive)continue;
			enemy2[i].pos.x = cosf(bossEnemyRotateTheta + attack2Pos.theta[enemyCount]) * attack2Pos.dstance[enemyCount] + bossEnemyPos.x;
			enemy2[i].pos.y = sinf(bossEnemyRotateTheta + attack2Pos.theta[enemyCount]) * attack2Pos.dstance[enemyCount] + bossEnemyPos.y;
			enemyCount++;
			if (enemyCount == 6)break;
		}
	}
	else if (bossEnemyNouStates != Attack2) {
		for (int i = 0;i < kEnemy2Limit;i++) {
			enemy2[i].isAlive = false;
			enemy2[i].hp = enemy2[i].hpMax;
		}
	}
}

#pragma endregion

#pragma region 関数: enemyBasic

static void InitEnemyBasic(void) {
	for (int i = 0; i < kEnemyBasicLimit; i++) {
		enemyBasic[i] = {};

		enemyBasic[i].texture = GetTexture(TextureType::EnemyBasic);
	}
}

/// <summary>
/// enemyBasicの移動処理
/// </summary>
/// <param name=""></param>
static void MoveEnemyBasic(void) {
	for (int i = 0; i < kEnemyBasicLimit; i++) {
		if (!enemyBasic[i].isAlive) continue;

		Vector2 targetPos = GetPlayerPos();
		float disX = targetPos.x - enemyBasic[i].pos.x;
		float disY = targetPos.y - enemyBasic[i].pos.y;
		float disLength = sqrtf(disX * disX + disY * disY);
		if (disLength != 0.0f) {
			disX /= disLength;
			disY /= disLength;
		}
		Vector2 moveVec = { disX, disY };

		enemyBasic[i].pos.x += moveVec.x * enemyBasic[i].moveSpeed;
		enemyBasic[i].pos.y += moveVec.y * enemyBasic[i].moveSpeed;
	}
}

/// <summary>
/// デンゲキショックの生成を受け付けない状態のカウント
/// </summary>
/// <param name=""></param>
static void CountIgnoreShockEnemyBasic(void) {
	for (int i = 0; i < kEnemyBasicLimit; i++) {
		if (!enemyBasic[i].isAlive) continue;
		if (!enemyBasic[i].isIgnoreShock) continue;


		enemyBasic[i].ignoreSpawnShockTimer.count++;
		if (enemyBasic[i].ignoreSpawnShockTimer.count >= enemyBasic[i].ignoreSpawnShockTimer.time) {
			enemyBasic[i].ignoreSpawnShockTimer.count = 0;
			enemyBasic[i].isIgnoreShock = false;
		}
	}
}

/// <summary>
/// 
/// </summary>
/// <param name=""></param>
static void UpdateFlashTimerEnemyBasic(void) {
	for (int i = 0; i < kEnemyBasicLimit; i++) {
		if (!enemyBasic[i].isAlive) continue;
		if (!enemyBasic[i].isFlash) continue;

		enemyBasic[i].flashTimer.count++;
		if (enemyBasic[i].flashTimer.count >= enemyBasic[i].flashTimer.time) {
			enemyBasic[i].flashTimer.count = 0;
			enemyBasic[i].isFlash = false;
		}
	}
}

static void DeadEnemyBasic(int index) {
	enemyBasic[index].isAlive = false;
	enemyBasic[index].hp = enemyBasic[index].hpMax;
	/*spawner.spawnTimer.time = spawner.kSpawnTime * nowenemyBasicIsAliveCount;*/
}

static void CheckDeadEnemyBasic(void) {
	for (int i = 0; i < kEnemyBasicLimit; i++) {
		if (!enemyBasic[i].isAlive) continue;

		if (enemyBasic[i].hp <= 0) {
			DeadEnemyBasic(i);

			//ジェムの生成処理
			for (int j = 0;j < enemyBasicGemLimit;j++) {
				SpawnGem(enemyBasic[i].pos, GemType::S);
			}
		}
	}
}

static void DrawEnemyBasic(void) {
	for (int i = 0; i < kEnemyBasicLimit; i++) {
		if (!enemyBasic[i].isAlive) continue;

		Texture texture = enemyBasic[i].texture;
		if (enemyBasic[i].isFlash) {
			texture.color = WHITE;
		}
		else {
			texture.color = kNormalColor;
		}

		DrawTextureObj(texture, enemyBasic[i].pos, enemyBasic[i].size);
	}
}


#pragma endregion

#pragma region 関数: enemyM

static void InitEnemyM(void) {
	for (int i = 0;i < kEnemyMLimit;i++) {
		enemyM[i] = {};

		enemyM[i].texture = GetTexture(TextureType::Enemy2);
	}
}

static void ShootEnemySmall(void) {
	for (int i = 0; i < kEnemyMLimit; i++) {
		if (!enemyM[i].isAlive) continue;

		enemyM[i].shootTimer.count++;
		if (enemyM[i].shootTimer.count >= enemy2[i].shootTimer.time) {
			enemyM[i].shootTimer.count = 0;
			for (int j = 0;j < kEnemySmallLimit;j++) {
				if (enemySmall[j].isAlive)continue;

				enemySmall[j].isAlive = true;
				enemySmall[j].velocity = { ToFloat(GetRand(0,5)),ToFloat(GetRand(0,5)) };
				enemySmall[j].pos = enemyM[i].pos;
				break;
			}
		}

	}
}

/// <summary>
/// デンゲキショックの生成を受け付けない状態のカウント
/// </summary>
/// <param name=""></param>
static void CountIgnoreShockEnemyM(void) {
	for (int i = 0; i < kEnemyMLimit; i++) {
		if (!enemyM[i].isAlive) continue;
		if (!enemyM[i].isIgnoreShock) continue;


		enemyM[i].ignoreSpawnShockTimer.count++;
		if (enemyM[i].ignoreSpawnShockTimer.count >= enemyM[i].ignoreSpawnShockTimer.time) {
			enemyM[i].ignoreSpawnShockTimer.count = 0;
			enemyM[i].isIgnoreShock = false;
		}
	}
}

static void UpdateFlashTimerEnemyM(void) {
	for (int i = 0; i < kEnemyMLimit; i++) {
		if (!enemyM[i].isAlive) continue;
		if (!enemyM[i].isFlash) continue;

		enemyM[i].flashTimer.count++;
		if (enemyM[i].flashTimer.count >= enemyM[i].flashTimer.time) {
			enemyM[i].flashTimer.count = 0;
			enemyM[i].isFlash = false;
		}
	}
}

static void DeadEnemyM(int index) {
	enemyM[index].isAlive = false;
	enemyM[index].hp = enemyM[index].hpMax;
}

static void CheckDeadEnemyM(void) {
	for (int i = 0; i < kEnemyMLimit; i++) {
		if (!enemyM[i].isAlive) continue;

		if (enemyM[i].hp <= 0) {
			DeadEnemyM(i);

			//ジェムの生成処理
			SpawnGem(enemyM[i].pos, GemType::S);
		}
	}
}

static void DrawEnemyM(void) {
	for (int i = 0; i < kEnemyMLimit; i++) {
		if (!enemyM[i].isAlive) continue;

		Texture texture = enemyM[i].texture;
		if (enemyM[i].isFlash) {
			texture.color = kFlashColor;
		}

		DrawTextureObj(texture, enemyM[i].pos, enemyM[i].size);
	}
}

#pragma endregion

#pragma region 関数: enemySmall

static void InitEnemySmoll(void) {
	for (int i = 0; i < kEnemySmallLimit; i++) {
		enemySmall[i] = {};

		enemySmall[i].texture = GetTexture(TextureType::EnemyBasic);
	}
}

/// <summary>
/// enemySmollの移動処理
/// </summary>
/// <param name=""></param>
static void MoveEnemySmall(void) {
	for (int i = 0; i < kEnemySmallLimit; i++) {
		if (!enemySmall[i].isAlive) continue;

		Vector2 targetPos = GetPlayerPos();
		float disX = targetPos.x - enemySmall[i].pos.x;
		float disY = targetPos.y - enemySmall[i].pos.y;
		float disLength = sqrtf(disX * disX + disY * disY);
		if (disLength != 0) {
			enemySmall[i].velocity.x = (enemySmall[i].velocity.x + disX / disLength) / 2;
			enemySmall[i].velocity.x = (enemySmall[i].velocity.y + disY / disLength) / 2;
			enemySmall[i].nowSpeed += enemySmall[i].accleretionSpeed;
			if (enemySmall[i].nowSpeed > enemySmall[i].moveSpeedLimit) {
				enemySmall[i].nowSpeed = enemySmall[i].moveSpeedLimit;
			}
			enemySmall[i].pos.x += enemySmall[i].velocity.x * enemySmall[i].nowSpeed;
			enemySmall[i].pos.y += enemySmall[i].velocity.y * enemySmall[i].nowSpeed;
		}

	}
}

/// <summary>
/// デンゲキショックの生成を受け付けない状態のカウント
/// </summary>
/// <param name=""></param>
static void CountIgnoreShockEnemySmall(void) {
	for (int i = 0; i < kEnemySmallLimit; i++) {
		if (!enemySmall[i].isAlive) continue;
		if (!enemySmall[i].isIgnoreShock) continue;


		enemySmall[i].ignoreSpawnShockTimer.count++;
		if (enemySmall[i].ignoreSpawnShockTimer.count >= enemySmall[i].ignoreSpawnShockTimer.time) {
			enemySmall[i].ignoreSpawnShockTimer.count = 0;
			enemySmall[i].isIgnoreShock = false;
		}
	}
}

/// <summary>
/// 
/// </summary>
/// <param name=""></param>
static void UpdateFlashTimerEnemySmall(void) {
	for (int i = 0; i < kEnemySmallLimit; i++) {
		if (!enemySmall[i].isAlive) continue;
		if (!enemySmall[i].isFlash) continue;

		enemySmall[i].flashTimer.count++;
		if (enemySmall[i].flashTimer.count >= enemySmall[i].flashTimer.time) {
			enemySmall[i].flashTimer.count = 0;
			enemySmall[i].isFlash = false;
		}
	}
}

static void DeadEnemySmoll(int index) {
	enemySmall[index].isAlive = false;
	enemySmall[index].hp = enemySmall[index].hpMax;
	/*spawner.spawnTimer.time = spawner.kSpawnTime * nowenemyBasicIsAliveCount;*/
}

static void CheckDeadEnemySmall(void) {
	for (int i = 0; i < kEnemySmallLimit; i++) {
		if (!enemySmall[i].isAlive) continue;

		if (enemySmall[i].hp <= 0) {
			DeadEnemySmoll(i);

			//ジェムの生成処理
			for (int j = 0;j < enemySmallGemLimit;j++) {
				SpawnGem(enemySmall[i].pos, GemType::S);
			}
		}
	}
}

static void DrawEnemySmoll(void) {
	for (int i = 0; i < kEnemySmallLimit; i++) {
		if (!enemySmall[i].isAlive) continue;

		Texture texture = enemySmall[i].texture;
		if (enemySmall[i].isFlash) {
			texture.color = WHITE;
		}
		else {
			texture.color = kNormalColor;
		}

		DrawTextureObj(texture, enemySmall[i].pos, enemySmall[i].size);
	}
}


#pragma region 関数: 生成処理

/// <summary>
/// 生成時の出現座標作成
/// </summary>
/// <param name="centerPos">中心座標</param>
/// <returns>出現座標</returns>
static Vector2 CreateScatterPos(Vector2 centerPos) {
	float scatterLength = ToFloat(GetRand(kScatterLengthMin, kScatterLengthMax));

	float theta = ToFloat(GetRand(0, 360)) * (ToFloat(M_PI) / 180.0f);

	Vector2 pos;
	pos.x = centerPos.x + scatterLength * cosf(theta);
	pos.y = centerPos.y + scatterLength * sinf(theta);

	return pos;
}

#pragma endregion

#pragma endregion

#pragma region 関数: enemyBomb

static void InitEnemyBomb(void) {
	for (int i = 0;i < kEnemy2Limit;i++) {
		enemyBomb[i] = {};

		enemyBomb[i].texture = GetTexture(TextureType::Enemy2);
	}
}


static void MoveEnemyBomb(void) {
	for (int i = 0;i < kEnemyBombLimit;i++) {
		if (enemyBomb[i].isAlive == true) {
			enemyBomb[i].nowSpeed -= enemyBomb[i].decelerationSpeed;
			if (enemyBomb[i].nowSpeed <= 0) {
				enemyBomb[i].nowSpeed = 0;
			}
			enemyBomb[i].pos.x += enemyBomb[i].velocity.x * enemyBomb[i].nowSpeed;
			enemyBomb[i].pos.y += enemyBomb[i].velocity.y * enemyBomb[i].nowSpeed;
			if (enemyBomb[i].pos.x <= 0) {
				enemyBomb[i].pos.x = 0;
				enemyBomb[i].velocity.x *= -1;
			}
			else if (enemyBomb[i].pos.x >= 2560) {
				enemyBomb[i].pos.x = 2560;
				enemyBomb[i].velocity.x *= -1;
			}
			if (enemyBomb[i].pos.y <= 0) {
				enemyBomb[i].pos.y = 0;
				enemyBomb[i].velocity.y *= -1;
			}
			else if (enemyBomb[i].pos.y >= 1440) {
				enemyBomb[i].pos.y = 1440;
				enemyBomb[i].velocity.y *= -1;
			}
		}
	}
}

static void DeadEnemyBomb(int index) {
	enemyBomb[index].isAlive = false;
	enemyBomb[index].hp = enemyBomb[index].hpMax;
}

static void BurstEnemyBomb(void) {
	Vector2 playerPos = GetPlayerPos();
	for (int i = 0;i < kEnemyBombLimit;i++) {
		enemyBomb[i].burst.count++;
		if (enemyBomb[i].burst.count == enemyBomb[i].burst.time) {
			float DiffX = enemyBomb[i].pos.x - playerPos.x;
			float DiffY = enemyBomb[i].pos.y - playerPos.y;
			float distance = sqrtf(DiffX * DiffX + DiffY * DiffY);
			if (distance <= enemyBomb[i].burstRadius) {
				PlayerDamage(enemyBomb[i].pos);
			}
			DeadEnemyBomb(i);
		}
	}
}


/// <summary>
/// デンゲキショックの生成を受け付けない状態のカウント
/// </summary>
/// <param name=""></param>
static void CountIgnoreShockEnemyBomb(void) {
	for (int i = 0; i < kEnemyBombLimit; i++) {
		if (!enemyBomb[i].isAlive) continue;
		if (!enemyBomb[i].isIgnoreShock) continue;


		enemyBomb[i].ignoreSpawnShockTimer.count++;
		if (enemyBomb[i].ignoreSpawnShockTimer.count >= enemyBomb[i].ignoreSpawnShockTimer.time) {
			enemyBomb[i].ignoreSpawnShockTimer.count = 0;
			enemyBomb[i].isIgnoreShock = false;
		}
	}
}

static void UpdateFlashTimerEnemyBomb(void) {
	for (int i = 0; i < kEnemyBombLimit; i++) {
		if (!enemyBomb[i].isAlive) continue;
		if (!enemyBomb[i].isFlash) continue;

		enemyBomb[i].flashTimer.count++;
		if (enemyBomb[i].flashTimer.count >= enemyBomb[i].flashTimer.time) {
			enemyBomb[i].flashTimer.count = 0;
			enemyBomb[i].isFlash = false;
		}
	}
}

static void CheckDeadEnemyBomb(void) {
	for (int i = 0; i < kEnemyBombLimit; i++) {
		if (!enemyBomb[i].isAlive) continue;

		if (enemyBomb[i].hp <= 0) {
			DeadEnemyBomb(i);

			//ジェムの生成処理
			SpawnGem(enemyBomb[i].pos, GemType::M);
		}
	}
}

static void DrawEnemyBomb(void) {
	for (int i = 0; i < kEnemyBombLimit; i++) {
		if (!enemyBomb[i].isAlive) continue;

		Texture texture = enemyBomb[i].texture;
		if (enemyBomb[i].isFlash) {
			texture.color = kFlashColor;
		}

		DrawTextureObj(texture, enemyBomb[i].pos, enemyBomb[i].size);
	}
}


#pragma endregion


void InitEnemy(void) {
	InitEnemy1();
	InitEnemy2();
	InitEnemyBasic();
	InitEnemyM();
	InitEnemySmoll();

	GetMovablePos(spawner.spawnRange);

}

void UpdateEnemy(void) {
	//enemy1
	CheckDeadEnemy1();
	MoveEnemy1();
	UpdateFlashTimerEnemy1();
	CountIgnoreShockEnemy1();
	InitEnemyBomb();

	//enemy2
	CheckDeadEnemy2();
	UpdateFlashTimerEnemy2();
	CountIgnoreShockEnemy2();
	ShootEnemyBullet2();

	//enemyBasic
	MoveEnemyBasic();
	CountIgnoreShockEnemyBasic();
	UpdateFlashTimerEnemyBasic();
	CheckDeadEnemyBasic();

	//enemyM
	CheckDeadEnemyM();
	UpdateFlashTimerEnemyM();
	CountIgnoreShockEnemyM();
	ShootEnemySmall();

	//enemySmoll
	MoveEnemySmall();
	CountIgnoreShockEnemySmall();
	UpdateFlashTimerEnemySmall();
	CheckDeadEnemySmall();

	//enemyBomb
	MoveEnemyBomb();
	BurstEnemyBomb();
	CountIgnoreShockEnemyBomb();
	UpdateFlashTimerEnemyBomb();
	CheckDeadEnemyBomb();

	ReSpawnEnemy();
	Attack2Enemy2Move();
}

void DrawEnemy(void) {
	DrawEnemy1();
	DrawEnemy2();
	DrawEnemyBasic();
	DrawEnemyM();
	DrawEnemySmoll();
	DrawEnemyBomb();
}

#pragma region 関数: 外部参照関係

void SpawnEnemy(Vector2 pos, EnemyType type) {
	switch (type) {
	case EnemyType::Enemy1: {
		for (int i = 0; i < kEnemy1Limit; i++) {
			if (enemy1[i].isAlive) continue;
			enemy1[i].isAlive = true;
			enemy1[i].pos = pos;
			nowEnemy1IsAliveCount++;
			spawner.spawnTimer.time = spawner.kSpawnTime * nowEnemy1IsAliveCount;
			break;
		}

		break;
	}
	case EnemyType::EnemyBasic: {
		for (int i = 0; i < kEnemyBasicLimit; i++) {
			if (enemyBasic[i].isAlive) continue;
			enemyBasic[i].isAlive = true;
			enemyBasic[i].pos = pos;
			//spawner.spawnTimer.time = spawner.kSpawnTime * nowenemyBasicIsAliveCount;
			break;
		}

		break;
	}
	case EnemyType::EnemySmall: {
		for (int i = 0; i < kEnemySmallLimit; i++) {
			if (enemySmall[i].isAlive) continue;

			enemySmall[i].isAlive = true;
			enemySmall[i].pos = pos;

			// 初期化（必要なら）
			enemySmall[i].nowSpeed = 0.0f;
			enemySmall[i].velocity = { 0,0 };

			break;
		}
		break;
	}
	case EnemyType::EnemySmallGroup: {
		for (int createNum = 0; createNum < 3; createNum++) {
			for (int i = 0; i < kEnemySmallLimit; i++) {
				if (enemySmall[i].isAlive) continue;

				enemySmall[i].isAlive = true;
				enemySmall[i].pos = CreateScatterPos(pos);

				// 初期化（必要なら）
				enemySmall[i].nowSpeed = 0.0f;
				enemySmall[i].velocity = { 0,0 };

				break;
			}
		}
		break;
	}
	case EnemyType::EnemyM: {
		for (int i = 0; i < kEnemyMLimit; i++) {
			if (enemyM[i].isAlive) continue;

			enemyM[i].isAlive = true;
			enemyM[i].pos = pos;

			// 初期化（必要なら）
			enemyM[i].shootTimer.count = 0;

			break;
		}
		break;
	}
	case EnemyType::EnemyBomb: {
		for (int i = 0;i < kEnemyBombLimit;i++) {
			if (enemyBomb[i].isAlive)continue;

			enemyBomb[i].isAlive = true;
			enemyBomb[i].pos = pos;

			enemyBomb[i].nowSpeed = ToFloat(GetRand(100, 170)) / 10;
			enemyBomb[i].velocity = { ToFloat(GetRand(-10, 10)),ToFloat(GetRand(-10,10)) };
			if (enemyBomb[i].velocity.x != 0) {
				enemyBomb[i].velocity.x /= 10;
			}
			if (enemyBomb[i].velocity.y != 0) {
				enemyBomb[i].velocity.y /= 10;
			}
			enemyBomb[i].burst.time = GetRand(200, 600);
			enemyBomb[i].burst.count = 0;

			break;
		}
		break;
	}
	}
}


Enemy1* GetEnemy1Array(void) {
	return enemy1;
}

int GetEnemy1Limit(void) {
	return kEnemy1Limit;
}

void DamageEnemy1(Enemy1& enemy, int pow) {
	enemy.hp -= pow;

	//フラッシュさせる
	enemy.isFlash = true;
	enemy.flashTimer.count = 0;
}

Enemy2* GetEnemy2Array(void) {
	return enemy2;
}

int GetEnemy2Limit(void) {
	return kEnemy2Limit;
}

void DamageEnemy2(Enemy2& enemy, int pow) {
	enemy.hp -= pow;

	//フラッシュさせる
	enemy.isFlash = true;
	enemy.flashTimer.count = 0;
}

EnemyBasic* GetEnemyBasicArray(void) {
	return enemyBasic;
}

int GetEnemyBasicLimit(void) {
	return kEnemyBasicLimit;
}

void DamageEnemyBasic(EnemyBasic& enemy, int pow) {
	enemy.hp -= pow;

	//フラッシュさせる
	enemy.isFlash = true;
	enemy.flashTimer.count = 0;
}

EnemyM* GetEnemyMArray(void) {
	return enemyM;
}

int GetEnemyMLimit(void) {
	return kEnemyMLimit;
}

void DamageEnemyM(EnemyM& enemy, int pow) {
	enemy.hp -= pow;

	//フラッシュさせる
	enemy.isFlash = true;
	enemy.flashTimer.count = 0;
}

EnemySmall* GetEnemySmallArray(void) {
	return enemySmall;
}

int GetEnemySmallLimit(void) {
	return kEnemySmallLimit;
}

void DamageEnemySmall(EnemySmall& enemy, int pow) {
	enemy.hp -= pow;

	//フラッシュさせる
	enemy.isFlash = true;
	enemy.flashTimer.count = 0;
}

#pragma endregion