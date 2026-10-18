#include "title.h"
#include "input.h"
#include "scene_manager.h"
#include "title_background.h"
#include "title_player.h"
#include "elec_bullet.h"
#include "effect.h"

#include "texture.h"
#include "se.h"
#include "vector2.h"
#include "common.h"
#include "tool.h"

#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

enum class ModeTitle {
	MovePlayer,
	TriggerLightUp,
	WaitInput,
};
ModeTitle nowMode = ModeTitle::MovePlayer;

#pragma region データ: ロゴ

struct Logo {
	Vector2 pos = { 640, 580 };
	Vector2 size = { 640, 256 };

	//ロゴの当たり判定の大きさ
	Vector2 hitSize = { 640, 200 };
	Vector2 hitVertex[4] = {};

	bool isLightUp = false;

	Texture texture = {};
};
static Logo logo;

#pragma endregion

#pragma region データ: 演出

struct BlackPanel {
	Vector2 pos = kWindowCenter;
	Vector2 size = { 1280, 720 };

	bool isShow = true;

	unsigned int color = 0x000000CC;

	Texture texture = {};
};
BlackPanel blackPanel;

struct FlashPanel {
	Vector2 pos = kWindowCenter;
	Vector2 size = { 1280, 720 };

	//<イージング>
	float easingT = 0.0f;
	float easingSpeed = 0.05f;
	float startFadeOutEasingT = 0.1f;
	EasingMode easingMode = EasingMode::EaseOut;

	//透明度
	int alpha = 0;
	int alphaMin = 0;
	int alphaMax = 255;

	Texture texture = {};
};
FlashPanel flashPanel;

#pragma endregion

#pragma region 関数: 項目決定

static void Confirm(void) {
	if (nowMode != ModeTitle::WaitInput) return;

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

#pragma region ロゴ関係

static void InitLogo(void) {
	logo = {};
	SetVertex(logo.pos, logo.hitSize, logo.hitVertex);
	logo.texture = GetTexture(TextureType::TitleLogo);
}

static void CheckCollisionVSElecBullet(void) {
	ElecBullet* elecBullet = GetElecBulletArray();
	int elecBulletLimit = GetElecBulletLimit();

	for (int j = 0; j < elecBulletLimit; j++) {
		if (!elecBullet[j].isShoot) continue;

		if (CheckCollisionBoxVSCircle(elecBullet[j].pos, elecBullet[j].hitRadius, logo.hitVertex[0], logo.hitVertex[3])) {
			BreakElecBullet(j);
			//ホウデンの生成
			SpawnElecShock(logo.pos, elecBullet[j].type, ElecShockOriginType::None, -1, ElecShockOriginType::None, -1);

			//モード切替
			nowMode = ModeTitle::TriggerLightUp;

		}
	}
}

static void UpdateLogo(void) {
	CheckCollisionVSElecBullet();
}



static void DrawLogo(void) {
	DrawTextureUI(logo.texture, logo.pos, logo.size);
}

#pragma endregion

#pragma region 暗転パネル

static void InitBlackPanel(void) {
	blackPanel = {};
	blackPanel.texture = GetTexture(TextureType::White1x1);
	blackPanel.texture.color = blackPanel.color;
}

static void DrawBlackPanel(void) {
	if (!blackPanel.isShow) return;

	DrawTextureUI(blackPanel.texture, blackPanel.pos, blackPanel.size);
}

#pragma endregion

#pragma region 関数: フラッシュ演出

static void InitFlashPanel(void) {
	flashPanel = {};
	flashPanel.texture = GetTexture(TextureType::White1x1);
}

static void UpdateFlashPanel(void) {
	if (nowMode != ModeTitle::TriggerLightUp) return;

	flashPanel.easingT += flashPanel.easingSpeed;
	if (flashPanel.easingT >= 1.0f) {
		flashPanel.easingT = 1.0f;
		nowMode = ModeTitle::WaitInput;
	}

	if (flashPanel.easingT < flashPanel.startFadeOutEasingT) {
		float t = (flashPanel.easingT - flashPanel.startFadeOutEasingT) / (1.0f - flashPanel.startFadeOutEasingT);
		if (t > 1.0f) t = 1.0f;

		flashPanel.alpha = UpdateValueByEasing(flashPanel.alphaMin, flashPanel.alphaMax, t, flashPanel.easingMode);
	} else {
		float t = (flashPanel.easingT - flashPanel.startFadeOutEasingT) / (1.0f - flashPanel.startFadeOutEasingT);
		if (t > 1.0f) t = 1.0f;

		flashPanel.alpha = UpdateValueByEasing(flashPanel.alphaMax, flashPanel.alphaMin, t, flashPanel.easingMode);

		//暗転パネルの削除
		blackPanel.isShow = false;
	}

}

static void DrawFlashPanel(void) {
	Texture texture = flashPanel.texture;
	texture.color = (texture.color & 0xFFFFFF00) | flashPanel.alpha;

	DrawTextureUI(texture, flashPanel.pos, flashPanel.size);
}

#pragma endregion

void InitTitle(void) {
	InitTitleBackground();

	InitTitlePlayer();
	InitElecBullet();
	InitEffect();

	InitLogo();
	InitFlashPanel();
	InitBlackPanel();
}

void UpdateTitle(void) {
	UpdateTitlePlayer();
	UpdateElecBullet();
	UpdateEffect();

	Confirm();
	UpdateLogo();
	UpdateFlashPanel();

	UpdateTitleBackground();
}

void DrawTitle(void) {
	DrawTitleBackground();
	DrawBlackPanel();

	DrawElecBullet();
	DrawEffect();

	DrawTitlePlayer();

	DrawLogo();

	DrawFlashPanel();
}