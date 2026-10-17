#include "title.h"
#include "input.h"
#include "scene_manager.h"
#include "texture.h"
#include "se.h"
#include "vector2.h"
#include "common.h"
#include "tool.h"

#include <Novice.h>

#pragma region データ: 背景

struct Background {
	Vector2 pos = kWindowCenter;
	Vector2 size = { 1280, 720 };

	unsigned int color = 0x1A020AFF;

	Texture texture = {};
};
Background background;

struct TitleDisplayEffect {
	Vector2 pos = kWindowCenter;
	Vector2 size = {};

	float easingT = 0.0f;
	float easingSpeed = 0.02f;
	EasingMode easingMode = EasingMode::EaseInOut;
	bool isScaleDown = true;

	Vector2 endSize = { 1280, 720 };
	Vector2 startSize = { 1440, 810 };

	int alpha = 30;
	int alphaMin = 30;
	int alphaMax = 100;

	int color = BLACK;

	Texture texture = {};
};
TitleDisplayEffect displayEffect;

static constexpr int kScanLineLimit = 21;

struct ScanLine {
	Vector2 pos = {};
	Vector2 size = { 1280, 2 };

	float moveSpeed = 1.0f;
	float posYMin = -0.2f;
	float posYMax = 722;

	float intervalY = 36.0f;

	int shakeDuration = 1;

	unsigned int color = 0x340314FF;

};
ScanLine scanLine[kScanLineLimit];
Texture scanLineTexture = {};



#pragma endregion

#pragma region 関数: 背景

static void InitBackground(void) {
	background = {};
	background.texture = GetTexture(TextureType::White1x1);

	for (int i = 0; i < kScanLineLimit; i++) {
		scanLine[i] = {};
		scanLine[i].pos = { 640, scanLine[i].intervalY * i };
	}
	scanLineTexture = GetTexture(TextureType::TitleLine);
}

static void UpdateScanLine(void) {
	for (int i = 0; i < kScanLineLimit; i++) {
		scanLine[i].pos.y += scanLine[i].moveSpeed;

		if (scanLine[i].pos.y >= scanLine[i].posYMax) {
			scanLine[i].pos.y = scanLine[i].posYMin;
		}
	}
}

static void DrawBackground(void) {
	//背景
	Texture texture = background.texture;
	texture.color = background.color;
	DrawTextureUI(texture, background.pos, background.size);


}

static void DrawScanLine(void) {
	for (int i = 0; i < kScanLineLimit; i++) {
		Vector2 pos;
		pos.y = scanLine[i].pos.y + ToFloat(GetRand(-scanLine[i].shakeDuration, scanLine[i].shakeDuration));

		Texture texture = scanLineTexture;
		texture.color = scanLine[i].color;

		DrawTextureUI(texture, scanLine[i].pos, scanLine[i].size);
	}
}

static void InitDisplayEffect(void) {
	displayEffect = {};
	displayEffect.texture = GetTexture(TextureType::DisplayEffect);
}

static void UpdateDisplayEffect(void) {
	//イージング処理
	if (displayEffect.isScaleDown) {
		displayEffect.easingT += displayEffect.easingSpeed;
		if (displayEffect.easingT >= 1.0f) {
			displayEffect.easingT = 1.0f;
			displayEffect.isScaleDown = false;
		}
	} else {
		displayEffect.easingT -= displayEffect.easingSpeed;
		if (displayEffect.easingT <= 0.0f) {
			displayEffect.easingT = 0.0f;
			displayEffect.isScaleDown = true;
		}
	}

	//サイズ変更
	displayEffect.size = MoveByEasing(displayEffect.startSize, displayEffect.endSize, displayEffect.easingT, displayEffect.easingMode);

	//不透明度変更
	displayEffect.alpha = UpdateValueByEasing(displayEffect.alphaMin, displayEffect.alphaMax, displayEffect.easingT, displayEffect.easingMode);
}

static void DrawDisplayEffect(void) {
	Texture texture = displayEffect.texture;
	texture.color = (displayEffect.color & 0xFFFFFF00) | displayEffect.alpha;

	DrawTextureUI(texture, displayEffect.pos, displayEffect.size);
}

#pragma endregion

#pragma region 関数: 項目決定

static void Confirm(void) {
	if (CheckInputAction(InputAction::Confirm)) {
		if (GetNowModeFade() != ModeFade::Standby) {
			return;
		}

		ChangeModeFade(ModeFade::FadeOut);
		ChangeFadeTarget(FadeTarget::Play);
		//PlaySe(SeType::Sample);
	}
}

#pragma endregion

void InitTitle(void) {
	InitBackground();
	InitDisplayEffect();
}

void UpdateTitle(void) {
	Confirm();


	UpdateScanLine();
	UpdateDisplayEffect();
}

void DrawTitle(void) {
	DrawBackground();

	DrawScanLine();
	DrawDisplayEffect();


}