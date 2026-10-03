#include "play.h"
#include "input.h"
#include "scene_manager.h"

#include "player.h"
#include "player_laser.h"
#include "stage.h"

#include <Novice.h>

static void Pause(void) {
	if (CheckInputAction(InputAction::Pause)) {
		ChangeScene(Scene::Pause);
	}
}


void InitPlay(void) {
	InitPlayer();
	InitPlayerLaser();

	InitStage();
}

void UpdatePlay(void) {
	UpdatePlayer();
	UpdatePlayerLaser();

	UpdateStage();


	Pause();
}

void DrawPlay(void) {
	DrawStage();

	DrawPlayer();
	DrawPlayerLaser();

#ifdef _DEBUG

	//Novice::ScreenPrintf(20, 20, "PlayScene");

#endif // _DEBUG
}