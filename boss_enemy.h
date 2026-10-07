#pragma once
#include "vector2.h"
#include "texture.h"


enum BossStates {
	Nomal,
	Attack1,
	Attack2,

	Count,
};

struct BossEnemy {
	Vector2 pos = { 500, 500 };
	Vector2 size = { 256, 256 };
	//<生存関係>
	bool isAlive = true;
	//当たり判定の大きさ
	float hitRadius =	128.0f;
	int remainLife = 1000;
	int remainLifeMax = 1000;
	//<移動関係>
	// 視界
	float viewRange = 400.0f;
	//速度
	Vector2 velocity = { 0.0f, 0.0f };
	//スピード
	float nowSpeed = 0.0f;
	//加速度
	float accleretionSpeed = 0.05f;
	//スピード上限
	float moveSpeedLimit = 2.0f;
	//減速度
	float decelerationSpeed = 0.12f;
	Vector2 inputVec = { 0, 0 };
	//<回転関係>
	float rotateTheta = 0.0f;
	//回転速度
	float rotateSpeed = 0.05f;
	Texture texture = {};
	//状態
	int nowsSates=BossStates::Attack1;
};


bool GetBossEnemyIsAlive(void);
int GetBossEnemyRemainLife(void);
int GetBossEnemyRemainLifeMax(void);

void InitBossEnemy(void);
void UpdateBossEnemy(void);	
void DrawBossEnemy(void);

BossEnemy* GetBossEnemy(void);