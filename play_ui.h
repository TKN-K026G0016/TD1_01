#pragma once
#include "vector2.h"

void InitPlayUI(void);
void UpdatePlayUI(void);
void DrawPlayUI(void);

/// <summary>
/// エネルギーゲージ残量の右下座標取得
/// </summary>
Vector2 GetEnergyGaugeEndPos(void);

/// <summary>
/// ボスHPゲージ残量の右下座標取得
/// </summary>
Vector2 GetBossHpGaugeEndPos(void);

/// <summary>
/// コンボの終了を知らせる
/// </summary>
void TriggerEndCombo(void);

void TriggerDisplayEffect(void);