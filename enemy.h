#pragma once
#include "vector2.h"
#include "texture.h"
#include "timer.h"
#include "texture.h"

//ザコ敵の種類
enum class EnemyType {
	//移動→射撃
	Enemy1,
	//ボス防御
	Enemy2,
	//移動のみ
	EnemyBasic,
	//小雑魚召喚
	EnemyM,
	//小雑魚
	EnemySmall,
	//小ザコの群体
	EnemySmallGroup,
	//ボム
	EnemyBomb,


	Count
};

struct Enemy1 {
	Vector2 pos = { 0, 0 };
	Vector2 size = { 64, 64 };

	//生存フラグ
	bool isAlive = false;

	int hp = 30;
	int hpMax = 30;

	float hitRadius = 20.0f;

	//<射撃処理関係>
	Timer shootTimer = { 300, 0 };

	//ショックの生成無視状態
	bool isIgnoreShock = false;
	//ショック生成無視タイマー
	Timer ignoreSpawnShockTimer = { 20, 0 };

	//<フラッシュ関係>
	bool isFlash = false;
	Timer flashTimer = { 7, 0 };


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


	Texture texture = {};
};

struct Enemy2 {
	Vector2 pos = { 0, 0 };
	Vector2 size = { 64, 64 };

	//生存フラグ
	bool isAlive = false;

	int hp = 60;
	int hpMax = 30;

	float hitRadius = 20.0f;

	//<射撃処理関係>
	Timer shootTimer = { 75, 0 };

	//ショックの生成無視状態
	bool isIgnoreShock = false;
	//ショック生成無視タイマー
	Timer ignoreSpawnShockTimer = { 20, 0 };

	//<フラッシュ関係>
	bool isFlash = false;
	Timer flashTimer = { 7, 0 };

	Texture texture = {};
};

struct EnemyBasic {
	Vector2 pos = { 0, 0 };
	Vector2 size = { 64, 64 };

	//生存フラグ
	bool isAlive = false;

	int hp = 10;
	int hpMax = 10;

	float hitRadius = 30.0f;

	//移動速度
	float moveSpeed = 1.5f;

	//ショックの生成無視状態
	bool isIgnoreShock = false;
	//ショック生成無視タイマー
	Timer ignoreSpawnShockTimer = { 20, 0 };

	//<フラッシュ関係>
	bool isFlash = false;
	Timer flashTimer = { 7, 0 };

	Texture texture = {};
};

struct EnemyM {
	Vector2 pos = { 0, 0 };
	Vector2 size = { 64, 64 };

	//生存フラグ
	bool isAlive = false;

	int hp = 60;
	int hpMax = 60;

	float hitRadius = 20.0f;

	//<射撃処理関係>
	Timer shootTimer = { 30, 0 };

	//ショックの生成無視状態
	bool isIgnoreShock = false;
	//ショック生成無視タイマー
	Timer ignoreSpawnShockTimer = { 20, 0 };

	//<フラッシュ関係>
	bool isFlash = false;
	Timer flashTimer = { 7, 0 };

	Texture texture = {};
};

struct EnemySmall {
	Vector2 pos = { 0, 0 };
	Vector2 size = { 16, 16 };

	//生存フラグ
	bool isAlive = false;

	int hp = 5;
	int hpMax = 5;

	float hitRadius = 20.0f;

	//<移動関係>
	//速度
	Vector2 velocity = { 0.0f, 0.0f };
	//スピード
	float nowSpeed = 0.0f;
	//加速度
	float accleretionSpeed = 0.05f;
	//スピード上限
	float moveSpeedLimit = 4.0f;
	//減速度
	float decelerationSpeed = 0.12f;
	//方向転換の比率
	float turnRate = 0.5f;

	//ショックの生成無視状態
	bool isIgnoreShock = false;
	//ショック生成無視タイマー
	Timer ignoreSpawnShockTimer = { 20, 0 };

	//<フラッシュ関係>
	bool isFlash = false;
	Timer flashTimer = { 7, 0 };

	Texture texture = {};
};

struct EnemyBomb {
	Vector2 pos = { 0, 0 };
	Vector2 size = { 64, 64 };
	//<生存関係>
	bool isAlive = false;

	int hp = 60;
	int hpMax = 60;

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

	//ショックの生成無視状態
	bool isIgnoreShock = false;
	//ショック生成無視タイマー
	Timer ignoreSpawnShockTimer = { 20, 0 };

	//<フラッシュ関係>
	bool isFlash = false;
	Timer flashTimer = { 7, 0 };


	Texture texture = {};
	Texture burstTexture = {};
};


void InitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

/// <summary>
/// ザコ敵の生成処理
/// </summary>
/// <param name="pos">生成座標</param>
/// <param name="type"生成する種類></param>
void SpawnEnemy(Vector2 pos, EnemyType type);

Enemy1* GetEnemy1Array(void);

int GetEnemy1Limit(void);

/// <summary>
/// Enemy1へのダメージ処理
/// </summary>
/// <param name="enemy"></param>
/// <param name="pow"></param>
void DamageEnemy1(Enemy1& enemy, int pow);

Enemy2* GetEnemy2Array(void);

int GetEnemy2Limit(void);

/// <summary>
/// Enemy2へのダメージ処理
/// </summary>
/// <param name="enemy"></param>
/// <param name="pow"></param>
void DamageEnemy2(Enemy2& enemy, int pow);


EnemyBasic* GetEnemyBasicArray(void);

int GetEnemyBasicLimit(void);

/// <summary>
/// EnemyBasicへのダメージ処理
/// </summary>
/// <param name="enemy">enemyBasic</param>
/// <param name="pow">ダメージ</param>
void DamageEnemyBasic(EnemyBasic& enemy, int pow);

EnemyM* GetEnemyMArray(void);

int GetEnemyMLimit(void);

/// <summary>
/// EnemyMへのダメージ処理
/// </summary>
/// <param name="enemy"></param>
/// <param name="pow"></param>
void DamageEnemyM(EnemyM& enemy, int pow);

EnemySmall* GetEnemySmallArray(void);

int GetEnemySmallLimit(void);

/// <summary>
/// EnemySmollへのダメージ処理
/// </summary>
/// <param name="enemy"></param>
/// <param name="pow"></param>
void DamageEnemySmall(EnemySmall&enemy, int pow);

EnemyBomb* GetEnemyBombArray(void);

int GetEnemyBombLimit(void);

/// <summary>
/// EnemyBombへのダメージ処理
/// </summary>
/// <param name="enemy"></param>
/// <param name="pow"></param>
void DamageEnemyBomb(EnemyBomb& enemy, int pow);