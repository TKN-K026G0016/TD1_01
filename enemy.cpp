#include "enemy.h"
#include "player.h"
#include "enemy_bullet.h"
#include "gem.h"

#include "vector2.h"
#include "timer.h"
#include "tool.h"
#include "texture.h"

#include <math.h>

constexpr int kEnemy1Limit = 10;

static Enemy1 enemy1[kEnemy1Limit];

struct Spawner {
	Timer spawnTimer = { 20, 0 };

	Vector2 spawnPos = {};
};
Spawner spawner;

#pragma region 関数: enemy1

static void InitEnemy1(void) {
	for (int i = 0; i < kEnemy1Limit; i++) {
		enemy1[i] = {};

		enemy1[i].texture = GetTexture(TextureType::Enemy1);
	}
}

static void ShootEnemyBullet(void) {
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

static void DeadEnemy1(int index) {
	enemy1[index].isAlive = false;
	enemy1[index].hp = enemy1[index].hpMax;
}

static void CheckDeadEnemy(void) {
	for (int i = 0; i < kEnemy1Limit; i++) {
		if (!enemy1[i].isAlive) continue;

		if (enemy1[i].hp <= 0) {
			DeadEnemy1(i);

			//ジェムの生成処理
			SpawnGem(enemy1[i].pos);
		}
	}
}

static void DrawEnemy1(void) {
	for (int i = 0; i < kEnemy1Limit; i++) {
		if (!enemy1[i].isAlive) continue;

		DrawTextureObj(enemy1[i].texture, enemy1[i].pos, enemy1[i].size);
	}
}

void ReSpawnEnemy(void) {
	spawner.spawnTimer.count++;
	if (spawner.spawnTimer.count == spawner.spawnTimer.time) {
		spawner.spawnTimer.count = 0;
		spawner.spawnPos = { ToFloat(GetRand(0, 2000)), ToFloat(GetRand(0, 1300)) };
		SpawnEnemy(spawner.spawnPos, EnemyType::Enemy1);
	}
}
#pragma endregion

void InitEnemy(void) {
	InitEnemy1();

	for (int i = 0; i < 5; i++) {
		Vector2 pos = { ToFloat(GetRand(0, 2000)), ToFloat(GetRand(0, 1300)) };

		SpawnEnemy(pos, EnemyType::Enemy1);
	}

}

void UpdateEnemy(void) {
	CheckDeadEnemy();
	ShootEnemyBullet();
	ReSpawnEnemy();
}

void DrawEnemy(void) {
	DrawEnemy1();
}

#pragma region 関数: 外部参照関係

void SpawnEnemy(Vector2 pos, EnemyType type) {
	switch (type) {
	case EnemyType::Enemy1:
		for (int i = 0; i < kEnemy1Limit; i++) {
			if (enemy1[i].isAlive) continue;
			enemy1[i].isAlive = true;
			enemy1[i].pos = pos;

			break;
		}

		break;
	}
}


Enemy1* GetEnemy1Array(void) {
	return enemy1;
}

int GetEnemy1Limit(void) {
	return kEnemy1Limit;
}

#pragma endregion