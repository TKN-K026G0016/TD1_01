
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

#pragma region データ: 共通

Texture whiteBoxTexture = {};

#pragma endregion

#pragma region データ: ジェム取得エフェクト

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

#pragma endregion

#pragma region データ: ゲージエフェクト

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

#pragma endregion

#pragma region 関数: 煙エフェクト
//煙エフェクトの数
static constexpr int kSmokeEffectLimit = 10;
//残像の数
static constexpr int kSmokeAfterimageLimit = 2;

struct SmokeEffect {
	Vector2 pos = {};
	//Vector2 size = { 24, 24 };

	bool isShoot = false;

	//移動速度
	float moveSpeed = 5.0f;

	//<サイズ関係>
	float firstSizeValue = 8;
	float sizeValue = firstSizeValue;
	float changeSizeSpeed = 4;

	//<回転関係>
	float rotateTheta = 0.0f;
	float rotateSpeed = ToFloat(M_PI) / 20.0f;

	//透明度関係
	int alpha = 220;
	int alphaMax = 220;
	int alphaMin = 0;
	int changeAlphaSpeed = 12;

	//<残像関係>
	float preSizeValue[kSmokeAfterimageLimit] = {};
	float preRotateTheta[kSmokeAfterimageLimit] = {};
	//本体との座標のずれ
	float posOffset = 10;
	//本体との透明度のズレ
	int diffAlpha = 30;

	Texture texture = {};
	unsigned int color = 0x333333FF;
};
SmokeEffect smokeEffect[kSmokeEffectLimit];

#pragma endregion

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

#pragma region 関数: smokeEffect

static void InitSmokeEffect(void) {
	for (int i = 0; i < kSmokeEffectLimit; i++) {
		smokeEffect[i] = {};
		smokeEffect[i].texture = GetTexture(TextureType::White1x1);
		smokeEffect[i].texture.color = smokeEffect[i].color;
	}
}

static void UpdateSmokeEffect(void) {
	for (int i = 0; i < kSmokeEffectLimit; i++) {
		if (!smokeEffect[i].isShoot) continue;

#pragma region 残像の更新処理
		//prePos(残像位置の更新)
		for (int j = 0; j < kSmokeAfterimageLimit; j++) {
			if (j == 0) {
				smokeEffect[i].preSizeValue[j] = smokeEffect[i].sizeValue;
				smokeEffect[i].preRotateTheta[j] = smokeEffect[i].rotateTheta;
			} else {
				smokeEffect[i].preSizeValue[j] = smokeEffect[i].preSizeValue[j - 1];
				smokeEffect[i].preRotateTheta[j] = smokeEffect[i].preRotateTheta[j - 1];
			}
		}

#pragma endregion

		//本体の移動処理
		smokeEffect[i].pos.y += smokeEffect[i].moveSpeed;

		//拡大処理
		smokeEffect[i].sizeValue += smokeEffect[i].changeSizeSpeed;
		//回転処理
		smokeEffect[i].rotateTheta += smokeEffect[i].rotateSpeed;
		//alpha値の変更
		smokeEffect[i].alpha -= smokeEffect[i].changeAlphaSpeed;

		//消滅処理
		if (smokeEffect[i].alpha <= smokeEffect[i].alphaMin) {
			smokeEffect[i].alpha = smokeEffect[i].alphaMin;
			smokeEffect[i].isShoot = false;
		}

	}
}

static void DrawSmokeEffect(void) {
	for (int i = 0; i < kSmokeEffectLimit; i++) {
		if (!smokeEffect[i].isShoot) continue;

		//残像の処理
		for (int j = 0; j < kSmokeAfterimageLimit; j++) {
			Vector2 pos = { smokeEffect[i].pos.x, smokeEffect[i].pos.y - smokeEffect[i].posOffset * (j + 1) };
			Vector2 size = { smokeEffect[i].preSizeValue[j], smokeEffect[i].preSizeValue[j] };

			Texture afterFadeTexture = smokeEffect[i].texture;
			
			//透明度の変更
			int alpha = smokeEffect[i].alpha - smokeEffect[i].diffAlpha * (j + 1);
			if (alpha < 0) {
				alpha = 0;
			}
			unsigned int color = (smokeEffect[i].texture.color & 0xFFFFFF00) | alpha;
			afterFadeTexture.color = color;


			DrawTextureRotateObj(afterFadeTexture, pos, size, smokeEffect[i].preRotateTheta[j]);
		}

		//本体の描写
		Vector2 size = { smokeEffect[i].sizeValue, smokeEffect[i].sizeValue };
		Texture texture = smokeEffect[i].texture;
		//透明度の変更
		unsigned int color = (smokeEffect[i].texture.color & 0xFFFFFF00) | smokeEffect[i].alpha;
		texture.color = color;

		DrawTextureRotateObj(texture, smokeEffect[i].pos, size, smokeEffect[i].rotateTheta);
	}
}

void CreateSmokeEffect(Vector2 pos) {
	for (int i = 0; i < kSmokeEffectLimit; i++) {
		if (smokeEffect[i].isShoot) continue;

		smokeEffect[i].isShoot = true;
		smokeEffect[i].pos = pos;

		//初期化
		smokeEffect[i].sizeValue = smokeEffect[i].firstSizeValue;
		smokeEffect[i].rotateTheta = 0.0f;
		smokeEffect[i].alpha = smokeEffect[i].alphaMax;
		for (int j = 0; j < kSmokeAfterimageLimit; j++) {
			//smokeEffect[i].prePos[j] = smokeEffect[i].pos;
			smokeEffect[i].preRotateTheta[j] = smokeEffect[i].rotateTheta;
		}

		break;
	}
}

#pragma endregion



void InitEffect() {
	InitGemEffect();
	
	InitGageEffect();

	InitSmokeEffect();

#ifdef _DEBUG

	CreateJemEffect();

	CreateGageEffect();

	CreateSmokeEffect({GetPlayerPos()});

#endif // _DEBUG

}
void UpdateEffect() {
	
	UpdateGemEffect();
	UodateGageEffect();
	UpdateSmokeEffect();

}
void DrawEffect() {
	DrawGetEffect();
	DrawGageEffect();
	DrawSmokeEffect();
}
