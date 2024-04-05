#pragma once
#include"WorldTransform.h"
#include"Model.h"

class GameScene;

class Pitfall
{
public:
	void Initialize(Model* model,Vector3 position);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	Vector3 GetPosition();

	Vector3 GetScale();

private:
	WorldTransform worldTransform_;

	Model* model_ =  nullptr ;

	GameScene* gameScene_ = nullptr;

};

