#include "pause.h"
#include "input.h"
#include "scene_manager.h"

#include "texture.h"
#include "tool.h"
#include "common.h"

#pragma region データ: 選択関係

enum class ModePauseSelect {
	Resume = 0,
	Retry = 1,
	Exit = 2,

	Count
};
static ModePauseSelect nowSelect = ModePauseSelect::Resume;
//既に決定してるかのフラグ
static bool onceSelect = false;

#pragma endregion

#pragma region データ: MenuBackground

struct MenuBackground {
	Vector2 pos = {};
	Vector2 size = { 1280, 720 };

	//イージング関係
	float easingT = 0.0f;
	float easingSpeed = 0.08f;
	EasingMode easingModeFrameIn = EasingMode::EaseIn;
	EasingMode easingModeFrameOut = EasingMode::EaseOut;

	//座標
	Vector2 standbyPos = { 1300, kWindowCenter.y };
	Vector2 showPos = kWindowCenter;

	//透明度
	int alpha = 200;

	Texture texture = {};
};
MenuBackground menuBackground;

#pragma endregion

#pragma region データ: アイコン関係

struct Icon {
	Vector2 pos = { 0, 0 };
	Vector2 size = { 0, 0 };

	Texture texture = {};
};
//ポーズアイコン
Icon pauseIcon;
static Vector2 pauseIconPos = { 640, 600 };
static Vector2 pauseIconSize = { 512, 128 };

//セレクトアイコンのサイズ
static Vector2 selectIconSize = { 256, 64 };

static Icon resumeIcon;
static Vector2 resumeIconPos = { 640, 200 };
static Icon retryIcon;
static Vector2 retryIconPos = { 640, 130 };
static Icon exitIcon;
static Vector2 exitIconPos = { 640, 60 };

#pragma endregion

#pragma region データ: カバーパネル

struct CoverPanel {
	Vector2 pos = kWindowCenter;
	Vector2 size = { 1280, 720 };

	unsigned int color = 0x00000088;

	Texture texture = {};
};
CoverPanel pauseCoverPanel = {};

#pragma endregion


#pragma region 関数: カバーパネル

static void InitCoverPanel(void) {
	//テクスチャデータの取得
	if (pauseCoverPanel.texture.tHandle != -1) return;

	pauseCoverPanel.texture = GetTexture(TextureType::White1x1);
	pauseCoverPanel.texture.color = pauseCoverPanel.color;
}

static void DrawCoverPanel(void) {
	DrawTextureUI(pauseCoverPanel.texture, pauseCoverPanel.pos, pauseCoverPanel.size);
}

#pragma endregion

#pragma region 関数: MenuBackground

static void InitMenuBackground(void) {
	menuBackground = {};
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

#pragma region 関数: 選択処理

static void InitSelect(void) {
	nowSelect = ModePauseSelect::Resume;
	onceSelect = false;
}

/// <summary>
/// ボタンによるポーズ解除処理
/// </summary>
/// <param name=""></param>
static void InputResume(void) {
	if (onceSelect) return;

	if (CheckInputAction(InputAction::Pause)) {
		onceSelect = true;
		nowSelect = ModePauseSelect::Resume;
	}
}

/// <summary>
/// カーソル移動処理
/// </summary>
static void ChangeCursor(void) {
	if (onceSelect) return;

	int selectNum = ToInt(nowSelect);

	if (CheckInputAction(InputAction::SelectUp)) {
		selectNum--;

		//一番上まで来ていたら、一番下へ
		if (selectNum < 0) {
			selectNum = ToInt(ModePauseSelect::Count) - 1;
		}

	} else if (CheckInputAction(InputAction::SelectDown)) {
		selectNum++;

		//一番下まで来ていたら、一番上へ
		if (selectNum >= ToInt(ModePauseSelect::Count)) {
			selectNum = 0;
		}
	}

	//変更を適用
	nowSelect = static_cast<ModePauseSelect>(selectNum);
}

/// <summary>
/// 決定処理
/// </summary>
static void Confirm(void) {
	if (onceSelect) return;

	if (CheckInputAction(InputAction::Confirm)) {
		onceSelect = true;
	}
}

/// <summary>
/// シーンの変更処理
/// </summary>
static void ChangeScene(void) {
	if (GetNowModeFade() != ModeFade::Standby) return;

	if (onceSelect && menuBackground.easingT <= 0.0f) {
		switch (nowSelect) {
		case ModePauseSelect::Resume: {
			ChangeSceneImmediate(Scene::Play);

			break;
		}
		case ModePauseSelect::Retry: {
			ChangeModeFade(ModeFade::FadeOut);
			ChangeFadeTarget(FadeTarget::Play);

			break;
		}
		case ModePauseSelect::Exit: {
			ChangeModeFade(ModeFade::FadeOut);
			ChangeFadeTarget(FadeTarget::Title);

			break;
		}
		}
	}
}

#pragma endregion

#pragma region 関数: アイコン

/// <summary>
/// アイコンの初期化処理
/// </summary>
static void InitIcon(void) {
	if (pauseIcon.texture.tHandle != -1) return;

	pauseIcon.pos = pauseIconPos;
	pauseIcon.size = pauseIconSize;
	pauseIcon.texture = GetTexture(TextureType::PauseIcon);

	resumeIcon.pos = resumeIconPos;
	resumeIcon.size = selectIconSize;
	resumeIcon.texture = GetTexture(TextureType::ResumeIcon);

	retryIcon.pos = retryIconPos;
	retryIcon.size = selectIconSize;
	retryIcon.texture = GetTexture(TextureType::RetryIcon);

	exitIcon.pos = exitIconPos;
	exitIcon.size = selectIconSize;
	exitIcon.texture = GetTexture(TextureType::ExitIcon);
}

static void UpdateIcon(void) {
	//resumeIcon
	if (nowSelect == ModePauseSelect::Resume) {
		resumeIcon.texture.animNum = 1;
	} else {
		resumeIcon.texture.animNum = 0;
	}

	//retry
	if (nowSelect == ModePauseSelect::Retry) {
		retryIcon.texture.animNum = 1;
	} else {
		retryIcon.texture.animNum = 0;
	}

	//exit
	if (nowSelect == ModePauseSelect::Exit) {
		exitIcon.texture.animNum = 1;
	} else {
		exitIcon.texture.animNum = 0;
	}
}

static void DrawIcon(void) {
	//PauseIcon
	DrawTextureUI(pauseIcon.texture, pauseIcon.pos, pauseIcon.size);

	//resumeIcon
	DrawTextureUI(resumeIcon.texture, resumeIcon.pos, resumeIcon.size);

	//retryIcon
	DrawTextureUI(retryIcon.texture, retryIcon.pos, retryIcon.size);

	//exitIcon
	DrawTextureUI(exitIcon.texture, exitIcon.pos, exitIcon.size);
}

#pragma endregion

void InitPause(void) {
	InitSelect();
	InitIcon();
	InitCoverPanel();
	InitMenuBackground();
}

void UpdatePause(void) {
	InputResume();

	ChangeCursor();
	Confirm();

	UpdateMenuBackground();
	UpdateIcon();

	ChangeScene();
}

void DrawPause(void) {
	DrawCoverPanel();
	DrawMenuBackground();

	DrawIcon();
}