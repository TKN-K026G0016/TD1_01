#include "clear.h"
#include "input.h"
#include "scene_manager.h"

#include "tool.h"
#include "texture.h"
#include "common.h"

#define _USE_MATH_DEFINES
#include <math.h>

enum class ModeClear {
	IconFrameIn,
	CountUpScore,
	CheckBreakScore,
	Select,
};
ModeClear nowMode = ModeClear::IconFrameIn;

#pragma region データ: 選択関係

enum class ModeClearSelect {
	Retry = 0,
	Exit = 1,

	Count
};
static ModeClearSelect nowSelect = ModeClearSelect::Retry;
//既に決定してるかのフラグ
static bool onceSelect = false;

#pragma endregion

#pragma region データ: MenuBackground

struct ClearMenuBackground {
	Vector2 pos = {};
	Vector2 size = { 1280, 720 };

	//イージング関係
	float easingT = 0.0f;
	float easingSpeed = 0.1f;
	EasingMode easingModeFrameIn = EasingMode::EaseOut;
	EasingMode easingModeFrameOut = EasingMode::EaseOut;

	//座標
	Vector2 standbyPos = { 1300, kWindowCenter.y };
	Vector2 showPos = kWindowCenter;

	//透明度
	int alpha = 200;

	Texture texture = {};
};
ClearMenuBackground menuBackground;

#pragma endregion

#pragma region データ: ボタン

struct Icon {
	Vector2 pos = { 0, 0 };
	Vector2 size = { 256, 64 };

	Texture texture = {};
};
static Icon retryIcon;
static Vector2 retryIconOffset = { 430, -140 };

static Icon exitIcon;
static Vector2 exitIconOffset = { 460, -210 };

struct ClearMenuCursor {
	Vector2 pos = {};
	Vector2 size = { 64, 64 };

	Vector2 offset = { -160, 0 };

	float rotateTheta = 0.0f;
	float rotateSpeed = ToFloat(M_PI) / 30.0f;

	Texture texture = {};
};
ClearMenuCursor clearMenuCursor;

#pragma endregion

#pragma region 関数: MenuBackground

static void InitMenuBackground(void) {
	menuBackground = {};
	menuBackground.pos = menuBackground.standbyPos;
	menuBackground.texture = GetTexture(TextureType::MenuBackground);
}

static void UpdateMenuBackground(void) {
	//Pause入ってからの処理
	if (!onceSelect) {
		if (menuBackground.easingT < 1.0f) {
			menuBackground.easingT += menuBackground.easingSpeed;
			if (menuBackground.easingT > 1.0f) {
				menuBackground.easingT = 1.0f;
			}
		}

		menuBackground.pos = MoveByEasing(menuBackground.standbyPos, menuBackground.showPos, menuBackground.easingT, menuBackground.easingModeFrameIn);
	}
	//選択後の処理
	else {
		if (menuBackground.easingT > 0.0f) {
			menuBackground.easingT -= menuBackground.easingSpeed;
			if (menuBackground.easingT < 0.0f) {
				menuBackground.easingT = 0.0f;
			}
		}

		menuBackground.pos = MoveByEasing(menuBackground.standbyPos, menuBackground.showPos, menuBackground.easingT, menuBackground.easingModeFrameOut);
	}
}

static void DrawMenuBackground(void) {
	Texture texture = menuBackground.texture;
	texture.color = (texture.color & 0xFFFFFF00) | menuBackground.alpha;

	DrawTextureUI(texture, menuBackground.pos, menuBackground.size);
}

#pragma endregion

#pragma region 関数: 選択関係

static void InitSelect(void) {
	nowSelect = ModeClearSelect::Retry;
	onceSelect = false;
}

/// <summary>
/// カーソルの移動処理
/// </summary>
static void ChangeCursor(void) {
	if (onceSelect) return;

	int selectNum = ToInt(nowSelect);

	if (CheckInputAction(InputAction::SelectUp)) {
		selectNum--;

		//一番上まで来ていたら、一番下へ
		if (selectNum < 0) {
			selectNum = ToInt(ModeClearSelect::Count) - 1;
		}

	} else if (CheckInputAction(InputAction::SelectDown)) {
		selectNum++;

		//一番下まで来ていたら、一番上へ
		if (selectNum >= ToInt(ModeClearSelect::Count)) {
			selectNum = 0;
		}
	}

	//変更を適用
	nowSelect = static_cast<ModeClearSelect>(selectNum);
}

/// <summary>
/// 決定処理
/// </summary>
static void Confirm(void) {
	if (onceSelect) return;

	if (CheckInputAction(InputAction::Confirm)) {
		switch (nowSelect) {
		case ModeClearSelect::Retry: {
			ChangeModeFade(ModeFade::FadeOut);
			ChangeFadeTarget(FadeTarget::Play);

			break;
		}
		case ModeClearSelect::Exit: {
			ChangeModeFade(ModeFade::FadeOut);
			ChangeFadeTarget(FadeTarget::Title);

			break;
		}
		}
	}
}

#pragma endregion

#pragma region 関数: アイコン関係

static void InitButton(void) {
	if (retryIcon.texture.tHandle != -1) return;

	retryIcon.texture = GetTexture(TextureType::RetryIcon);

	exitIcon.texture = GetTexture(TextureType::ExitIcon);

	clearMenuCursor.texture = GetTexture(TextureType::MenuCursor);
}

static void UpdateButton(void) {

	//retryIconの処理
	retryIcon.pos = { menuBackground.pos.x + retryIconOffset.x, menuBackground.pos.y + retryIconOffset.y };
	//exitIconの処理
	exitIcon.pos = { menuBackground.pos.x + exitIconOffset.x, menuBackground.pos.y + exitIconOffset.y };

#pragma region 点灯処理
	//RetryIcon
	if (nowSelect == ModeClearSelect::Retry) {
		retryIcon.texture.animNum = 1;
	} else {
		retryIcon.texture.animNum = 0;
	}

	//exitIcon
	if (nowSelect == ModeClearSelect::Exit) {
		exitIcon.texture.animNum = 1;
	} else {
		exitIcon.texture.animNum = 0;
	}
#pragma endregion
}

static void UpdateCursor(void) {

	Vector2 targetPos = {};
	switch (nowSelect) {

	case ModeClearSelect::Retry:
		targetPos = { retryIcon.pos.x + clearMenuCursor.offset.x,retryIcon.pos.y + clearMenuCursor.offset.y };
		break;

	case ModeClearSelect::Exit:
		targetPos = { exitIcon.pos.x + clearMenuCursor.offset.x,exitIcon.pos.y + clearMenuCursor.offset.y };
		break;
	}

	clearMenuCursor.pos = targetPos;

	//回転処理
	clearMenuCursor.rotateTheta -= clearMenuCursor.rotateSpeed;

}

static void DrawButton(void) {
	DrawTextureUI(retryIcon.texture, retryIcon.pos, retryIcon.size);

	DrawTextureUI(exitIcon.texture, exitIcon.pos, exitIcon.size);

	DrawTextureUI(clearMenuCursor.texture, clearMenuCursor.pos, clearMenuCursor.size);
}

#pragma endregion

void InitClear(void) {
	InitMenuBackground();
	InitButton();
	InitSelect();
}

void UpdateClear(void) {
	switch (nowMode) {
		case ModeClear::IconFrameIn:


			break;
		case ModeClear::CountUpScore:


			break;
		case ModeClear::CheckBreakScore:


			break;
		case ModeClear::Select:
			UpdateMenuBackground();

			ChangeCursor();
			Confirm();

			break;
	}

	UpdateButton();
}

void DrawClear(void) {
	DrawMenuBackground();

	DrawButton();
}