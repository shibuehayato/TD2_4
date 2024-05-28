#pragma once
#include "WorldTransform.h"
#include"Model.h"
#include"ImGuiManager.h"
#include"Stage3RotateCannonBullet.h"
#include"ImGuiManager.h"
class GameScene;
class Stage3RotateConnon
{
public:
	void Initialize(Model* modelHead, Model* model, Model* modelbullet);

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

	Model* model_[3] = { nullptr };
	GameScene* gameScene_ = nullptr;
	Stage3RotateCannonBullet* stage3rotatecannonbullet = nullptr;

	float rotation_ = 0.1f;
	bool isrotation_ = false;

	// 発射タイマー
	int32_t kFireTimer_ = 0;
};

