#pragma once
#include "vector2.h"

enum class PlayerLaserLevel {
	Level0 = 0,
	Level1 = 1,
	Level2 = 2,

	Count
};

void InitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

/// <summary>
/// playerの座標取得
/// </summary>
Vector2 GetPlayerPos(void);

/// <summary>
/// playerの当たり判定の大きさ取得
/// </summary>
/// <returns>player.hitRadius</returns>
float GetPlayerHitRadius(void);

/// <summary>
/// playerの体力取得
/// </summary>
/// <returns>player.remainLife</returns>
int GetPlayerRemainLife(void);

/// <summary>
/// playerの生存フラグ取得
/// </summary>
/// <returns>player.isAlive</returns>
bool GetPlayerIsAlive(void);

/// <summary>
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
float GetPlayerRemainEnergy(void);

/// <summary>
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
float GetPlayerEnergyLimit(void);

/// <summary>
/// エネルギー回復処理
/// </summary>
/// <param name="recoveryValue">回復量</param>
void RecoveryEnergy(float recoveryValue);

/// <summary>
/// playerの方向取得
/// </summary>
/// <returns>player.rotateTheta</returns>
float GetPlayerRotateTheta(void);

/// <summary>
/// playerのレベル取得
/// </summary>
/// <returns>player.laserLevel</returns>
PlayerLaserLevel GetPlayerNowLaserLevel(void);

/// <summary>
/// playerの被弾処理
/// </summary>
/// <param name=""></param>
void PlayerDamage(void);