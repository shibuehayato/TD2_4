#pragma once
#include"WorldTransform.h"
#include"Model.h"

class GameScene;
class Stage3Wall
{
public:
	void Initialize(Model* model,Vector3 position);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void SetGameScene(GameScene* gameScene) { gamescene_ = gameScene; }

	Vector3 GetPosition();

	Vector3 GetScale();

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	GameScene* gamescene_ = nullptr;
};

