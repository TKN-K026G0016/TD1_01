#pragma once
#include "vector2.h"

//頂点の数
const int vertexLimit = 4;

template <typename T>
//static_cast<int>()の代わり
constexpr int ToInt(T value) {
	return static_cast<int>(value);
}

template <typename T>
//static_cast<float>()の代わり
constexpr float ToFloat(T value) {
	return static_cast<float>(value);
}

void InitTool(void);

/// <summary>
/// ランダムなcutinを取得
/// </summary>
/// <param name="min">最小値</param>
/// <param name="max">最大値</param>
/// <returns></returns>
int GetRand(int min, int max);

/// <summary>
/// ワールド座標をスクリーン座標に変換
/// </summary>
/// <param name="posW">ワールド座標</param>
/// <returns>スクリーン座標</returns>
Vector2i ConvertPosWToS(Vector2 posW);

/// <summary>
/// ワールド座標をスクリーン座標に変換(UI)
/// </summary>
/// <param name="posW">ワールド座標</param>
/// <returns>スクリーン座標</returns>
Vector2i ConvertPosWToSUI(Vector2 posW);

/// <summary>
/// ワールド座標からスクリーン座標への変換(背景用)
/// </summary>
/// <param name="posW">ワールド座標</param>
/// <param name="delay">スクロールとの倍率</param>
/// <returns>スクリーン座標</returns>
Vector2i ConvertPosWToSForBG(Vector2 posW, float scrollRate);

/// <summary>
/// ワールド座標からスクリーン座標への変換(HUD用)
/// </summary>
/// <param name="posW">ワールド座標</param>
/// <returns>スクリーン座標</returns>
Vector2 ConvertPosWToSForUI(Vector2 posW);

/// <summary>
/// 頂点座標の設定
/// </summary>
/// <param name="pos">中心座標</param>
/// <param name="size">大きさ</param>
/// <param name="vertexNum">頂点の番号</param>
/// <returns>頂点座標</returns>
Vector2 SetVertex(Vector2 pos, Vector2 size, int vertexNum);

/// <summary>
/// CircleVSCircleの当たり判定
/// </summary>
/// <param name="circle1Pos">円1の座標</param>
/// <param name="radius1">円1の半径</param>
/// <param name="circle2Pos">円2の座標</param>
/// <param name="radius2">円2の半径</param>
/// <returns>接触しているか</returns>
bool CheckCollisionCircleVSCircle(Vector2 circle1Pos, float radius1, Vector2 circle2Pos, float radius2);

/// <summary>
/// 矩形と円の当たり判定
/// </summary>
/// <param name="circlePos">円の座標</param>
/// <param name="circleRadius">円の半径</param>
/// <param name="boxVertex0">矩形の頂点座標(左上)</param>
/// <param name="boxVertex3">矩形の頂点座標(右下)</param>
/// <returns>衝突しているか</returns>
bool CheckCollisionBoxVSCircle(Vector2 circlePos, float circleRadius, Vector2 boxVertex0, Vector2 boxVertex3);

enum class EasingMode {
	//一定の速度
	Linear,
	//前半に加速
	EaseIn,
	//後半に減速
	EaseOut,
	EaseInOut,
};

Vector2 MoveByEasing(Vector2 startPos, Vector2 endPos, float t, EasingMode mode);

int UpdateValueByEasing(int startNum, int endNum, float t, EasingMode mode);

void DrawHitBox(Vector2 hitVertex0, Vector2 hitVertex1);

/// <summary>
/// 指定の桁の数値を取得
/// </summary>
/// <param name="targetNum">目的の数値</param>
/// <param name="targetDigit">取りたい桁(右端は0)</param>
/// <returns>取りたい桁の数値</returns>
int GetValueAtDigit(int targetNum, int targetDigit);