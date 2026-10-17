#include "score_data.h"

#pragma region データ
//現在のスコア
int nowScore = 0;
//ハイスコア
int highScore = 0;

#pragma endregion

void InitScoreData(void) {

}

void UpdateScoreData(void) {

}

int GetNowScore(void) {
	return nowScore;
}

int GetHighScore(void) {
	return highScore;
}

void AddScore(int point) {
	nowScore += point;
}

void ResetNowScore(void) {
	nowScore = 0;
}

void UpdateHighScore(void) {
	highScore = nowScore;
}