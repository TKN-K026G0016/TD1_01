#pragma once
#include "vector2.h"

void InitEffect();
void UpdateEffect();
void DrawEffect();

void CreateJemEffect();
void CreateGageEffect();


/// <summary>
/// 煙エフェクトの生成処理
/// </summary>
/// <param name="pos">生成座標</param>
void CreateSmokeEffect(Vector2 pos);