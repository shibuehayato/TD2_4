#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include "Model.h"

class GameScene;

class Stage2Recovery
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void OnCollision();

	void SetGameScene(GameScene* gamescene) { gamescene_ = gamescene; }

	bool IsDead() const { return isDead_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();
	// 大きさ取得
	Vector3 GetRadius() { return WorldTransform_.scale_; };



private:
	WorldTransform WorldTransform_;
	

	Model* Model_ = nullptr;

	bool isDead_ = false;

	GameScene* gamescene_ = nullptr;
};

