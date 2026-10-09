#pragma once

void InitElecBullet(void);
void UpdateElecBullet(void);
void DrawElecBullet(void);

/// <summary>
/// デンゲキ弾の発射処理
/// </summary>
/// <param name="pos">発射元の座標</param>
/// <param name="moveTheta">進行方向</param>
/// <param name="firstDisLength">最初のプレイヤーとの距離</param>
void ShootElecBullet(Vector2 pos, float moveTheta);

/// <summary>
/// デンゲキ弾の破棄処理
/// </summary>
/// <param name="index">番号</param>
void BreakElecBullet(int index);