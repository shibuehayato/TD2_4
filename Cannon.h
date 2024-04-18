#pragma once
#include "WorldTransform.h"
#include"Model.h"
#include"ImGuiManager.h"
#include"Cannonbullet.h"
class GameScene;
class Cannon
{
public:
	void Initialize(Model* modelHead,Model* model,Vector3 position,Vector3 HeadPosition,Vector3 rotation);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void FiringInitialize();

	void FiringUpdate();

	//弾攻撃
	void Fire();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	Vector3 GetPosition();

	Vector3 GetScale();

	// 発射間隔
	static const int kFireInterval_ = 60;

private:
	WorldTransform worldTransformHead_;
	WorldTransform worldTransform_;

	Model* model_[2] = { nullptr };
	GameScene* gameScene_ = nullptr;
	Cannonbullet* cannonbullet = nullptr;

	// 発射タイマー
	int32_t kFireTimer_ = 0;
};

