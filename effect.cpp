
#include "effect.h"
#include "player.h"
#include "vector2.h"
#include "texture.h"
#include "play_ui.h"
#include "timer.h"
#include "tool.h"


#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

//データ
const int KJemEffectNumber=10;
struct JemEffect
{
	Vector2 pos = { };
	Vector2 newPos = { };
	float theta = { };
	Vector2  size = { };
	unsigned int color = {0xffffffff };
	bool isShot = {false};
	Texture texture = {};
};
JemEffect jemEffect[KJemEffectNumber];

const int KGageEffectNumber = 20;
struct GageEffect
{
	Vector2 pos = { };
	Vector2 acceleration = { };
	Vector2 size = {32,32 };
	bool appear = { };
	unsigned int color = { };
	Texture texture = {};
};
GageEffect gageEffect[KGageEffectNumber];
bool isGage = false;
Timer GageTimer = {5,0};



#pragma region 関数: ジェム取得effect



void CreateJemEffect() {
	for (int i = 0; i < KJemEffectNumber; i++)
	{
		if (jemEffect[i].isShot == false) {
			jemEffect[i].isShot = true;
			jemEffect[i].pos = GetPlayerPos();
			jemEffect[i].theta = static_cast<float>((M_PI) / 4);
			jemEffect[i].size.x = 0;
			jemEffect[i].size.y = 0;
			jemEffect[i].color = 0xffffffff;
			break;
		}
	}
}

static void InitGemEffect(void) {
	for (int i = 0; i < KJemEffectNumber; i++) {
		jemEffect[i].texture = GetTexture(TextureType::EffectSquare);
	}
}

static void UpdateGemEffect(void) {
	for (int i = 0; i < KJemEffectNumber; i++)
	{
		if (jemEffect[i].isShot == true) {
			if (jemEffect[i].size.x <= 128&&jemEffect[i].size.y<=128) {
				jemEffect[i].size.y += 20;
				jemEffect[i].size.x += 20;
			}
			

			jemEffect[i].newPos.x = jemEffect[i].pos.x + (-(jemEffect[i].size.x / 2)) * cosf(jemEffect[i].theta) - (-(jemEffect[i].size.x / 2)) * sinf(jemEffect[i].theta);
			jemEffect[i].newPos.y = jemEffect[i].pos.y + (-(jemEffect[i].size.y / 2)) * cosf(jemEffect[i].theta) + (-(jemEffect[i].size.y / 2)) * sinf(jemEffect[i].theta);
			
			if (jemEffect[i].color >= 0xffffff10) {
				if (jemEffect[i].size.x < 128&&jemEffect[i].size.y<128) {
					jemEffect[i].color -= 0x00000005;
				}
				else {
					jemEffect[i].color -= 0x00000015;
				}
				jemEffect[i].texture.color = jemEffect[i].color;
			}
			else {
				jemEffect[i].color = 0xffffffff;
				jemEffect[i].isShot = false;
			}
		}
	}
}
void DrawGetEffect() {
	for (int i = 0; i < KJemEffectNumber; i++)
	{
		if(jemEffect[i].isShot==true){
		DrawTextureRotateObj(jemEffect[i].texture, jemEffect[i].pos, jemEffect[i].size,jemEffect[i].theta);
		}
	}
}

#pragma endregion

#pragma region 関数: ゲージeffect
void CreateGageEffect() {
	if (GageTimer.count > 0) {
		GageTimer.count--;
	}

	if (isGage == true) {
		if (GageTimer.count >= 0) {
			for (int i = 0; i < KGageEffectNumber; i++)
			{
				if (gageEffect[i].appear) {
					gageEffect[i].appear = false;
					GageTimer.count = 20;
					gageEffect[i].pos = GetEnergyGaugeEndPos();
					gageEffect[i].color = 0xffffffff;
					float randomSpeedX = ToFloat(GetRand(0, 25));
					float randomSpeedY = ToFloat(GetRand(-10, 10));
					gageEffect[i].acceleration.x = randomSpeedX;
					gageEffect[i].acceleration.y = randomSpeedY;



				}
			}
		}
	}
}

static void InitGageEffect(void) {
	for (int i = 0; i < KJemEffectNumber; i++) {
		gageEffect[i].texture = GetTexture(TextureType::EffectSquareMini);
	}
}

VOID UodateGageEffect() {
	for (int i = 0; i < KGageEffectNumber; i++)
	{
		gageEffect[i].pos.x += gageEffect[i].acceleration.x;
		gageEffect[i].pos.y += gageEffect[i].acceleration.y;
		gageEffect[i].acceleration.y -= 0.5f;
		if (gageEffect[i].color >= 0xffffff05) {
			gageEffect[i].color -= 0x00000015;
			gageEffect[i].texture.color = gageEffect[i].color;
		}
		else {
			gageEffect[i].color = 0xffffffff;
			gageEffect[i].appear = false;
		}
	}
}

void DrawGageEffect() {
	for (int i = 0; i < KGageEffectNumber; i++)
	{
		if (gageEffect[i].appear == true) {
			DrawTextureObj(gageEffect[i].texture, gageEffect[i].pos, gageEffect[i].size);
		}
	}
}
#pragma endregion

#pragma region



void InitEffect() {
	InitGemEffect();
	CreateJemEffect();
	InitGageEffect();
	CreateGageEffect();

}
void UpdateEffect() {
	
	UpdateGemEffect();
	UodateGageEffect();

}
void DrawEffect() {
	DrawGetEffect();
	DrawGageEffect();
}
