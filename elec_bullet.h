#pragma once
#include "vector2.h"
#include "timer.h"
#include "texture.h"

//デンゲキ弾
struct ElecBullet {
	Vector2 pos;
	Vector2 size = { 32, 32 };

	//<移動関係>
	float moveSpeed = 20.0f;
	float moveTheta = 0.0f;

	//<射撃フラグ>
	bool isShoot = false;
	Timer breakTimer = { 60, 0 };

	//当たり判定
	float hitRadius = 20;

	//攻撃力
	int pow = 2;

	Texture texture = {};
};

//デンゲキショック
struct ElecShock {
	Vector2 pos = {};

	bool isShoot = false;

	//<当たり判定の大きさ>
	float hitRadius = 120.0f;
	int pow = 2;

	Timer breakTimer = { 7, 0 };
	int triggerDamageFrame = 1;

	unsigned int color = RED;
};

void InitElecBullet(void);
void UpdateElecBullet(void);
void DrawElecBullet(void);

/// <summary>
/// デンゲキ弾の発射処理
/// </summary>
/// <param name="pos">発射元の座標</param>
/// <param name="moveTheta">進行方向</param>
/// <param name="firstDisLength">最初のプレイヤーとの距離</param>
void ShootElecBullet(Vector2 pos, float moveTheta);

/// <summary>
/// デンゲキ弾の破棄処理
/// </summary>
/// <param name="index">番号</param>
void BreakElecBullet(int index);

/// <summary>
/// elecBullet配列の取得
/// </summary>
/// <param name=""></param>
/// <returns></returns>
ElecBullet* GetElecBulletArray(void);

/// <summary>
/// kElecBulletLimitの取得
/// </summary>
/// <returns></returns>
int GetElecBulletLimit(void);

/// <summary>
/// デンゲキショックの生成処理
/// </summary>
/// <param name="pos"></param>
void SpawnElecShock(Vector2 pos);

/// <summary>
/// デンゲキショックの破壊処理
/// </summary>
/// <param name="index"></param>
void BreakElecShock(int index);

/// <summary>
/// デンゲキショックの配列取得
/// </summary>
/// <param name=""></param>
/// <returns></returns>
ElecShock* GetElecShockArray(void);

/// <summary>
/// デンゲキショックの配列取得
/// </summary>
/// <param name=""></param>
/// <returns></returns>
int GetElecShockLimit(void);