#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"ImGuiManager.h"

class GameScene;

class Barrier2
{
public:
	void Initialize(Model* model, Vector3 position);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void OnCollision();

	void Reset();

	//ゲームシーンからアドレスをもらうための関数
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//ワールド座標を取得
	Vector3 GetPosition();

	//ワールドサイズを取得
	Vector3 GetScale();

	bool IsDead() { return isDead_; }

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	GameScene* gameScene_ = nullptr;

	bool isDead_ = false;

	bool isOnCollision_ = false;

};



