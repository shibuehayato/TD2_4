#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"ImGuiManager.h"
#include <Input.h>

class Player;
class GameScene;

class Stage2RotatingArrow
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void OnCollision(Player* player);

	// ワールド座標を取得
	Vector3 GetWorldPosition() {

		// ワールド座標を入れる変数
		Vector3 worldPos{};
		worldTransform_.matWorld_.m;
		// ワールド行列の平行移動成分を取得(ワールド座標)
		worldPos.x = worldTransform_.matWorld_.m[3][0];
		worldPos.y = worldTransform_.matWorld_.m[3][1];
		worldPos.z = worldTransform_.matWorld_.m[3][2];
		return worldPos;
	}
	// 大きさ取得
	Vector3 GetRadius() { return worldTransform_.scale_; };


	//ゲームシーンからアドレスをもらうための関数
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	// 速度
	Vector3 velocity_;

	//矢印の回転に合わせるよう
	Vector3 ArrowRot;
	Vector3 ArrowRotspeed;

	Input* input_ = nullptr;

	GameScene* gameScene_ = nullptr;
};

