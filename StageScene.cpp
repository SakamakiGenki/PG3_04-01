#include "StageScene.h"
#include <Novice.h>
#include <algorithm>
#include <cmath>
#include <vector>

// ゲーム用の構造体
struct Vector2 {
	float x, y;
};

struct Bullet {
	Vector2 pos;
	float speed;
	bool isActive;
};

struct Player {
	Vector2 pos;
	int radius;
};

struct Enemy {
	Vector2 pos;
	int radius;
	bool isAlive;
};

// 定数
const int kWindowWidth = 1280;
const int kWindowHeight = 720;
const int kMaxBullet = 10;

// グローバル（staticでも可）
Player player = {
    {640, 600},
    20
};
Enemy enemy = {
    {640, 100},
    20, true
};

std::vector<Bullet> bullets;
int bulletCoolTime = 0;

void StageScene::Initialize() {
	player.pos = {640, 600};
	player.radius = 20;

	enemy.pos = {640, 100};
	enemy.radius = 20;
	enemy.isAlive = true;

	bullets.clear();
	bulletCoolTime = 0;
}

void StageScene::Update(char* keys, char* preKeys) {
	// スペースキーで次のシーンへ
	if (preKeys[DIK_SPACE] == 0 && keys[DIK_SPACE] != 0) {
		sceneNo = CLEAR;
	}

	// プレイヤー移動
	if (keys[DIK_LEFT]) {
		player.pos.x -= 5;
	}
	if (keys[DIK_RIGHT]) {
		player.pos.x += 5;
	}
	if (keys[DIK_UP]) {
		player.pos.y -= 5;
	}
	if (keys[DIK_DOWN]) {
		player.pos.y += 5;
	}

	// 画面外制限
	if (player.pos.x < 0)
		player.pos.x = 0;
	if (player.pos.x > kWindowWidth)
		player.pos.x = (float)kWindowWidth;
	if (player.pos.y < 0)
		player.pos.y = 0;
	if (player.pos.y > kWindowHeight)
		player.pos.y = (float)kWindowHeight;

	// 弾発射（Zキー）
	if (keys[DIK_Z] && bulletCoolTime <= 0) {
		if ((int)bullets.size() < kMaxBullet) {
			Bullet newBullet = {
			    {player.pos.x, player.pos.y - player.radius},
                -10.0f, true
            };
			bullets.push_back(newBullet);
			bulletCoolTime = 15;
		}
	}
	if (bulletCoolTime > 0)
		bulletCoolTime--;

	// 弾の更新処理
	for (auto& bullet : bullets) {
		if (bullet.isActive) {
			bullet.pos.y += bullet.speed;

			// 当たり判定（enemyが生きているなら）
			if (enemy.isAlive) {
				float dx = bullet.pos.x - enemy.pos.x;
				float dy = bullet.pos.y - enemy.pos.y;
				float dist = sqrtf(dx * dx + dy * dy);

				if (dist < player.radius + enemy.radius) {
					bullet.isActive = false;
					enemy.isAlive = false;
				}
			}

			// 画面外
			if (bullet.pos.y < 0) {
				bullet.isActive = false;
			}
		}
	}

	// 非アクティブ弾の削除
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return !b.isActive; }), bullets.end());
}

void StageScene::Draw() {
	Novice::ScreenPrintf(0, 0, "Current Scene : Stage");
	Novice::ScreenPrintf(0, 30, "Push Space : Next Scene");
	Novice::ScreenPrintf(0, 60, "Arrow: Move / Z: Shoot");

	// プレイヤー描画（緑）
	Novice::DrawEllipse((int)player.pos.x, (int)player.pos.y, player.radius, player.radius, 0.0f, 0x00FF00FF, kFillModeSolid);

	// 弾描画（白）
	for (const auto& bullet : bullets) {
		if (bullet.isActive) {
			Novice::DrawEllipse((int)bullet.pos.x, (int)bullet.pos.y, 5, 5, 0.0f, 0xFFFFFFFF, kFillModeSolid);
		}
	}

	// 敵描画（赤）
	if (enemy.isAlive) {
		Novice::DrawEllipse((int)enemy.pos.x, (int)enemy.pos.y, enemy.radius, enemy.radius, 0.0f, 0xFF0000FF, kFillModeSolid);
	}
}