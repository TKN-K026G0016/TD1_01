#include "texture.h"
#include "timer.h"
#include "tool.h"

#include <Novice.h>
#include <algorithm>

Texture textures[ToInt(TextureType::Count)] = {};

void InitTexture(void) {
	Texture temp[ToInt(TextureType::Count)] = {

		//サンプル
		//テクスチャデータ, 参照サイズ, animChangeTimer(time, count), animLimit(4枚アニメなら3), animNum, color
		//{Novice::LoadTexture("./resources/player/player_body.png"), { 128, 128 }, {0, 0}, 0, 0, WHITE },

		//===============
		//Title
		//===============
		{ Novice::LoadTexture("./NoviceResources/white1x1.png"), { 1, 1 }, {0, 0}, 0, 0, WHITE },

		//===============
		//Title
		//===============


		//===============
		//Play
		//===============
		//Player
		{ Novice::LoadTexture("./resources/texture/player/player.png"), { 128, 128 }, {0, 0}, 0, 0, WHITE },

		{ Novice::LoadTexture("./resources/texture/player/laser1.png"), { 64, 64 }, {0, 0}, 0, 0, WHITE },

		//Enemy

		//MapChip

		//background
		{ Novice::LoadTexture("./resources/texture/stage/background.png"), { 2560, 1440 }, {0, 0}, 0, 0, WHITE },

		//UI

		//===============
		//Gameover
		//===============

	};

	memcpy(textures, temp, sizeof(textures));

}


Texture GetTexture(TextureType type) {
	return textures[ToInt(type)];
}

/// <summary>
/// オブジェクトの描画関数
/// </summary>
/// <param name="texture">テクスチャデータ</param>
/// <param name="centerPos">中心座標</param>
/// <param name="size">サイズ</param>
void DrawTextureObj(Texture texture, Vector2 centerPos, Vector2 size) {

	Vector2 vertexW[kVertexNum] = { 0, 0 };
	Vector2i vertexS[kVertexNum] = { 0, 0 };

	SetVertex(centerPos, size, vertexW);

	for (int i = 0; i < kVertexNum; i++) {
		vertexS[i] = ConvertPosWToS(vertexW[i]);
	}

	Novice::DrawQuad(
		vertexS[0].x, vertexS[0].y,
		vertexS[1].x, vertexS[1].y,
		vertexS[2].x, vertexS[2].y,
		vertexS[3].x, vertexS[3].y,
		texture.refSize.x * texture.animNum, 0, texture.refSize.x, texture.refSize.y,
		texture.tHandle, texture.color
	);
}

/// <summary>
/// オブジェクト(反転)の描画関数
/// </summary>
/// <param name="texture">テクスチャデータ</param>
/// <param name="centerPos">中心座標</param>
/// <param name="size">サイズ</param>
void DrawTextureObjReverse(Texture texture, Vector2 centerPos, Vector2 size) {

	Vector2 vertexW[kVertexNum] = { 0, 0 };
	Vector2i vertexS[kVertexNum] = { 0, 0 };

	SetVertex(centerPos, size, vertexW);

	for (int i = 0; i < kVertexNum; i++) {
		vertexS[i] = ConvertPosWToS(vertexW[i]);
	}

	Novice::DrawQuad(
		vertexS[0].x, vertexS[0].y,
		vertexS[1].x, vertexS[1].y,
		vertexS[2].x, vertexS[2].y,
		vertexS[3].x, vertexS[3].y,
		texture.refSize.x * (texture.animNum + 1), 0, -texture.refSize.x, texture.refSize.y,
		texture.tHandle, texture.color
	);
}

/// <summary>
/// 回転するオブジェクトの描画関数
/// </summary>
/// <param name="texture">テクスチャデータ</param>
/// <param name="centerPos">中心座標</param>
/// <param name="size">サイズ</param>
/// <param name="rotateTheta">回転量</param>
void DrawTextureRotateObj(Texture texture, Vector2 centerPos, Vector2 size, float rotateTheta) {

	Vector2 vertexW[kVertexNum] = { 0, 0 };
	Vector2i vertexS[kVertexNum] = { 0, 0 };
	//頂点座標を更新
	SetVertexRotate(centerPos, size, vertexW, rotateTheta);
	//ワールド座標からスクリーン座標に変換
	for (int i = 0; i < kVertexNum; i++) {
		vertexS[i] = ConvertPosWToS(vertexW[i]);
	}

	Novice::DrawQuad(
		vertexS[0].x, vertexS[0].y,
		vertexS[1].x, vertexS[1].y,
		vertexS[2].x, vertexS[2].y,
		vertexS[3].x, vertexS[3].y,
		texture.refSize.x * texture.animNum, 0, texture.refSize.x, texture.refSize.y,
		texture.tHandle, texture.color
	);
}

/// <summary>
/// 背景の描画関数
/// </summary>
/// <param name="texture">テクスチャデータ</param>
/// <param name="centerPos">中心座標</param>
/// <param name="size">サイズ</param>
/// <param name="scrollRate">スクロール倍率</param>
void DrawTextureBG(Texture texture, Vector2 centerPos, Vector2 size, float scrollRate) {

	Vector2 vertexW[kVertexNum] = { 0, 0 };
	Vector2i vertexS[kVertexNum] = { 0, 0 };

	SetVertex(centerPos, size, vertexW);

	for (int i = 0; i < kVertexNum; i++) {
		vertexS[i] = ConvertPosWToSForBG(vertexW[i], scrollRate);
	}

	Novice::DrawQuad(
		vertexS[0].x, vertexS[0].y,
		vertexS[1].x, vertexS[1].y,
		vertexS[2].x, vertexS[2].y,
		vertexS[3].x, vertexS[3].y,
		texture.refSize.x * texture.animNum, 0, texture.refSize.x, texture.refSize.y,
		texture.tHandle, texture.color
	);
}

/// <summary>
/// UI(HUD)の描画関数
/// </summary>
/// <param name="texture">テクスチャデータ</param>
/// <param name="centerPos">中心座標</param>
/// <param name="size">サイズ</param>
void DrawTextureUI(Texture texture, Vector2 centerPos, Vector2 size) {
	Vector2 vertexW[kVertexNum] = { 0, 0 };
	Vector2i vertexS[kVertexNum] = { 0, 0 };

	SetVertex(centerPos, size, vertexW);

	for (int i = 0; i < kVertexNum; i++) {
		vertexS[i] = ConvertPosWToSUI(vertexW[i]);
	}

	Novice::DrawQuad(
		vertexS[0].x, vertexS[0].y,
		vertexS[1].x, vertexS[1].y,
		vertexS[2].x, vertexS[2].y,
		vertexS[3].x, vertexS[3].y,
		texture.refSize.x * texture.animNum, 0, texture.refSize.x, texture.refSize.y,
		texture.tHandle, texture.color
	);
}

/// <summary>
/// ゲージの描画処理(Obj)
/// </summary>
/// <param name="texture">テクスチャデータ</param>
/// <param name="centerPos">中心座標</param>
/// <param name="size">サイズ</param>
/// <param name="rate">ゲージの割合</param>
void DrawGaugeAsObj(Texture texture, Vector2 centerPos, Vector2 size, float rate) {
	//割合に補正を掛ける
	rate = std::clamp(rate, 0.0f, 1.0f);
	float gaugeWidth = size.x * rate;
	int gaugeRefWidth = ToInt(texture.refSize.x * rate);

	Vector2 vertexW[kVertexNum] = { 0, 0 };
	Vector2i vertexS[kVertexNum] = { 0, 0 };

	SetVertex(centerPos, size, vertexW);

	for (int i = 0; i < kVertexNum; i++) {
		//右側の頂点をrateに合わせてx座標更新
		if (i == 1) {
			vertexW[i].x = vertexW[0].x + gaugeWidth;
		} else if (i == 3) {
			vertexW[i].x = vertexW[2].x + gaugeWidth;
		}

		vertexS[i] = ConvertPosWToS(vertexW[i]);
	}

	Novice::DrawQuad(
		vertexS[0].x, vertexS[0].y,
		vertexS[1].x, vertexS[1].y,
		vertexS[2].x, vertexS[2].y,
		vertexS[3].x, vertexS[3].y,
		 texture.refSize.x * texture.animNum, 0, gaugeRefWidth, texture.refSize.y,
		texture.tHandle, texture.color
	);
}

/// <summary>
/// アニメーション変更処理(通常)
/// </summary>
/// <param name="texture">テクスチャデータ</param>
void UpdateAnimation(Texture& texture ) {
	texture.animChangeTimer.count++;
	if (texture.animChangeTimer.count >= texture.animChangeTimer.time) {
		texture.animChangeTimer.count = 0;

		if (texture.animNum < texture.animLimit) {
			texture.animNum++;
		}
		else {
			texture.animNum = 0;
		}

	}
}