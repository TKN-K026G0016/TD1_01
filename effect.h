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


/// <summary>
/// ゲージエフェクトの出現処理
/// </summary>
/// <param name="pos">生成座標</param>
void CreateGageEffect(Vector2 pos);

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

/// <summary>
/// ChargeOrbエフェクト生成処理
/// </summary>
/// <param name="pos">生成座標</param>
void CreateChargeOrbEffect(Vector2 pos);

/// <summary>
/// ChargeChipエフェクト生成処理
/// </summary>
void CreateChargeChipEffect();

/// <summary>
/// 爆発エフェクトの生成処理
/// </summary>
/// <param name="pos">生成座標</param>
void CreateExplodeEffect(Vector2 pos);

/// <summary>
///三角形のエフェクト生成処理　
/// </summary>
/// /// <param name="pos"></param>
void CreateTriangleEffect(Vector2 pos);

/// <summary>
/// 拡大する円のエフェクト生成処理
/// </summary>
/// <param name="pos"></param>
void CreateCircleEffect(Vector2 pos);