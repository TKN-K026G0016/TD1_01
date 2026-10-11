#pragma once
#include "vector2.h"

//エフェクトの描画タイプ
enum class TypeDrawEffect {
	Obj,
	UI,
};

void InitEffect();
void UpdateEffect();
void DrawEffect();
void DrawEffectAsUI();

void CreateJemEffect();
void CreateGageEffect();


/// <summary>
/// 煙エフェクトの生成処理
/// </summary>
/// <param name="pos">生成座標</param>
void CreateSmokeEffect(Vector2 pos, TypeDrawEffect drawType);

/// <summary>
/// ElecShockエフェクト生成処理
/// </summary>
/// <param name="pos">生成座標</param>
void CreateElecShockEffect(Vector2 pos);

/// <summary>
/// ElecShock(チャージ)エフェクト生成処理
/// </summary>
/// <param name="pos">生成座標</param
void CreateElecShockChargedEffect(Vector2 pos);