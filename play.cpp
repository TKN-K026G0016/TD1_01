#include "play.h"
#include "input.h"
#include "scene_manager.h"

#include "player.h"
#include "stage.h"

#include <Novice.h>

static void Pause(void) {
	if (CheckInputAction(InputAction::Pause)) {
		ChangeScene(Scene::Pause);
	}
}


void InitPlay(void) {
	InitPlayer();

	InitStage();
}

void UpdatePlay(void) {

	UpdatePlayer();

	UpdateStage();


	Pause();
}

void DrawPlay(void) {

	DrawStage();
	DrawPlayer();

#ifdef _DEBUG

	//Novice::ScreenPrintf(20, 20, "PlayScene");

#endif // _DEBUG
}