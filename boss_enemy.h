#pragma once
#include "vector2.h"
#include "texture.h"

#define _USE_MATH_DEFINES
#include <math.h>


enum BossStates {
	//移動のみ
	Normal,
	//爆弾
	Attack1,
	//ザコ召喚
	Attack2,
	//レーザー射撃
	Attack3,

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

	//状態
	int nowsSates=BossStates::Attack2;

	Texture texture = {};
};

struct Attack2Enemy2Pos {
	float dstance[6] = {
		175,
		150,
		175,
		225,
		225,
		250,
	};
	float theta[6] = {
		(1.0f / 4.0f) * float(M_PI),
		0,
		(-1.0f/4.0f)*float(M_PI),
		(1.0f/8.0f)*float(M_PI),
		(-1.0f/8.0f)*float(M_PI),
		0
	};
};


bool GetBossEnemyIsAlive(void);
Vector2 GetBossEnemyPos(void);
float GetBossEnemyRotateTheta(void);
int GetBossEnemyRemainLife(void);
int GetBossEnemyRemainLifeMax(void);
int GetBossEnemyNowStates(void);
Timer GetAttack2Timer(void);

void InitBossEnemy(void);
void UpdateBossEnemy(void);	
void DrawBossEnemy(void);

BossEnemy* GetBossEnemy(void);