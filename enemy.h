#pragma once
#include "vector2.h"
#include "texture.h"
#include "timer.h"
#include "texture.h"

//ザコ敵の種類
enum class EnemyType {
	Enemy1,
	Enemy2,

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