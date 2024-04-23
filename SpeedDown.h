#pragma once
#include"WorldTransform.h"
#include"Model.h"

class GameScene;
class SpeedDown
{
public:
	void Initialize(Model* model,Vector3 position);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void SetGameScene(GameScene* gamescene) { gameScene_ = gamescene; }

	Vector3 GetPosition();

	Vector3 GetScale();

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	GameScene* gameScene_=nullptr;

	uint32_t textureHandle_ = 0u;
};

