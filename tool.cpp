#include "tool.h"
#include "vector2.h"
#include "common.h"
#include "stage.h"

#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

void InitTool(void) {
	srand(ToInt(time(nullptr)));
}


int GetRand(int min, int max) {
	return (rand() % (max - min + 1)) + min;
};

Vector2 SetVertex(Vector2 pos, Vector2 size, int vertexNum) {
	Vector2 vertexPos = { 0, 0 };

	switch (vertexNum) {
	case 0:
	{
		vertexPos = { pos.x - size.x / 2, pos.y + size.y / 2 };

		break;
	}
	case 1:
	{
		vertexPos = { pos.x + size.x / 2, pos.y + size.y / 2 };

		break;
	}
	case 2:
	{
		vertexPos = { pos.x - size.x / 2, pos.y - size.y / 2 };

		break;
	}
	case 3:
	{
		vertexPos = { pos.x + size.x / 2, pos.y - size.y / 2 };

		break;
	}
	}

	//vertexPos.x *= cameraZoom;
	//vertexPos.y *= cameraZoom;

	return vertexPos;
}

Vector2i ConvertPosWToS(Vector2 posW) {

	Vector2 posS = { 0, 0 };
	Vector2 cameraPos = GetCameraPos();
	// まず通常のスクリーン座標へ変換
	posS.x = (posW.x + kDelayBetweenWToS.x + cameraPos.x);
	posS.y = (-posW.y + kDelayBetweenWToS.y + cameraPos.y);

	// ウィンドウ中心
	float centerX = kWindowSize.x / 2.0f;
	float centerY = kWindowSize.y / 2.0f;

	// 中心基準で拡縮
	posS.x = (posS.x - centerX) * GetCameraZoom() +centerX;
	posS.y = (posS.y - centerY) * GetCameraZoom() +centerY;

	return { ToInt(posS.x), ToInt(posS.y) };
}

Vector2i ConvertPosWToSUI(Vector2 posW) {

	Vector2 posS = { 0, 0 };
	// まず通常のスクリーン座標へ変換
	posS.x = (posW.x + kDelayBetweenWToS.x);
	posS.y = (-posW.y + kDelayBetweenWToS.y);

	return { ToInt(posS.x), ToInt(posS.y) };
}

/// <summary>
/// ワールド座標からスクリーン座標への変換(背景用)
/// </summary>
/// <param name="posW">ワールド座標</param>
/// <returns>スクリーン座標</returns>
Vector2i ConvertPosWToSForBG(Vector2 posW, float scrollRate) {
	Vector2 posS;
	Vector2 cameraPos = GetCameraPos();

	// まず通常のスクリーン座標へ変換
	posS.x = (posW.x + kDelayBetweenWToS.x + cameraPos.x * scrollRate);
	posS.y = (-posW.y + kDelayBetweenWToS.y + cameraPos.y * scrollRate);

	// ウィンドウ中心
	float centerX = kWindowSize.x / 2.0f;
	float centerY = kWindowSize.y / 2.0f;

	// 中心基準で拡縮
	posS.x = (posS.x - centerX) * GetCameraZoom() +centerX;
	posS.y = (posS.y - centerY) * GetCameraZoom() +centerY;

	return { ToInt(posS.x), ToInt(posS.y) };
}

bool CheckCollisionCircleVSCircle(Vector2 circle1Pos, float radius1, Vector2 circle2Pos, float radius2) {
	float disX = circle1Pos.x - circle2Pos.x;
	float disY = circle1Pos.y - circle2Pos.y;
	float disTotal = sqrtf(disX * disX + disY * disY);

	float disRad = radius1 + radius2;

	if (disTotal <= disRad) {
		return true;
	} else {
		return false;
	}

}


bool CheckCollisionBoxVSCircle(Vector2 circlePos, float circleRadius, Vector2 boxVertex0, Vector2 boxVertex3) {
	float closestPosX = std::clamp(circlePos.x, boxVertex0.x, boxVertex3.x);
	float closestPosY = std::clamp(circlePos.y, boxVertex3.y, boxVertex0.y);

	float disX = circlePos.x - closestPosX;
	float disY = circlePos.y - closestPosY;
	float disTotal = sqrtf(disX * disX + disY * disY);

	if (disTotal <= circleRadius) {
		return true;
	} else {
		return false;
	}
}

/// <summary>
/// 線形補正での移動
/// </summary>
/// <param name="startPos">startPos</param>
/// <param name="endPos">endPos</param>
/// <param name="t">媒介変数(この関数の前に更新しておく)</param>
/// <param name="mode">動き方</param>
/// <returns>更新後のPos</returns>
Vector2 MoveByEasing(Vector2 startPos, Vector2 endPos, float t, EasingMode mode) {
	float easedT = 0.0f;

	switch (mode) {
	case EasingMode::Linear:
	{
		easedT = t;

		break;
	}
	case  EasingMode::EaseIn:
	{
		easedT = t * t;

		break;
	}
	case  EasingMode::EaseOut:
	{
		easedT = 1.0f - powf(1.0f - t, 3.0f);

		break;
	}
	case  EasingMode::EaseInOut:
	{
		easedT = -(cosf(static_cast<float>(M_PI) * t) - 1.0f) / 2.0f;

		break;
	}
	}

	if (easedT > 1.0f) {
		easedT = 1.0f;
	}

	return { (1.0f - easedT) * startPos.x + easedT * endPos.x,(1.0f - easedT) * startPos.y + easedT * endPos.y };
}

/// <summary>
/// 線形補正での数値の更新
/// </summary>
/// <param name="startPos">最初の数値</param>
/// <param name="endPos">最終的な数値</param>
/// <param name="t">媒介変数(この関数の前に更新しておく)</param>
/// <param name="mode">動き方</param>
/// <returns>更新後の数値</returns>
int UpdateValueByEasing(int startNum, int endNum, float t, EasingMode mode) {
	float easedT = 0.0f;

	switch (mode) {
	case  EasingMode::Linear:
	{
		easedT = t;

		break;
	}
	case  EasingMode::EaseIn:
	{
		easedT = t * t;

		break;
	}
	case  EasingMode::EaseOut:
	{
		easedT = 1.0f - powf(1.0f - t, 3.0f);

		break;
	}
	case  EasingMode::EaseInOut:
	{
		easedT = -(cosf(static_cast<float>(M_PI) * t) - 1.0f) / 2.0f;

		break;
	}
	}

	if (easedT > 1.0f) {
		easedT = 1.0f;
	}

	return static_cast<int>((1.0f - easedT) * startNum + easedT * endNum);
}

/// <summary>
/// 矩形の当たり判定描画
/// </summary>
/// <param name="hitVertex0">左上の頂点座標</param>
/// <param name="hitVertex1">右下の頂点座標</param>
void DrawHitBox(Vector2 hitVertex0, Vector2 hitVertex1) {
	Vector2i hitBoxVertexS[2];
	hitBoxVertexS[0] = ConvertPosWToS(hitVertex0);
	hitBoxVertexS[1] = ConvertPosWToS(hitVertex1);

	Novice::DrawLine
	(
		hitBoxVertexS[0].x, hitBoxVertexS[0].y,
		hitBoxVertexS[1].x, hitBoxVertexS[0].y, RED
	);
	Novice::DrawLine
	(
		hitBoxVertexS[0].x, hitBoxVertexS[0].y,
		hitBoxVertexS[0].x, hitBoxVertexS[1].y, RED
	);
	Novice::DrawLine
	(
		hitBoxVertexS[0].x, hitBoxVertexS[1].y,
		hitBoxVertexS[1].x, hitBoxVertexS[1].y, RED
	);
	Novice::DrawLine
	(
		hitBoxVertexS[1].x, hitBoxVertexS[0].y,
		hitBoxVertexS[1].x, hitBoxVertexS[1].y, RED
	);
}

/// <summary>
/// 指定の桁の数値を取得
/// </summary>
/// <param name="targetNum">目的の数値</param>
/// <param name="targetDigit">取りたい桁(右端は0)</param>
/// <returns>取りたい桁の数値</returns>
int GetValueAtDigit(int targetNum, int targetDigit) {
	int digit;

	digit = targetNum / static_cast<int>(pow(10, targetDigit));
	digit %= 10;

	return digit;
}