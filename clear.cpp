#include "clear.h"
#include "input.h"
#include "scene_manager.h"

#include "tool.h"
#include "texture.h"
#include "common.h"

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

#pragma region データ: アイコン

struct Icon {
	Vector2 pos = { 0, 0 };
	Vector2 size = { 256, 64 };

	Texture texture = {};
};
static Icon retryIcon;
static Vector2 retryIconPos = { 640, 200 };

static Icon exitIcon;
static Vector2 exitIconPos = { 640, 130 };

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

static void InitIcon(void) {
	if (retryIcon.texture.tHandle != -1) return;

	retryIcon.pos = retryIconPos;
	retryIcon.texture = GetTexture(TextureType::RetryIcon);

	exitIcon.pos = exitIconPos;
	exitIcon.texture = GetTexture(TextureType::ExitIcon);
}

static void UpdateIcon(void) {
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
}

static void DrawIcon(void) {
	DrawTextureUI(retryIcon.texture, retryIcon.pos, retryIcon.size);

	DrawTextureUI(exitIcon.texture, exitIcon.pos, exitIcon.size);
}

#pragma endregion

void InitClear(void) {
	InitIcon();
	InitSelect();
}

void UpdateClear(void) {
	ChangeCursor();
	Confirm();

	UpdateIcon();
}

void DrawClear(void) {
	DrawIcon();
}