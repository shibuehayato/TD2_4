#pragma once
#include"WorldTransform.h"
#include"Model.h"

class GameScene;

class Wind
{
public:
	void Initialize(Model* model,Vector3 position);

	void Update();

	void Draw(ViewProjection& viewProjection);

	//ゲームシーンからアドレスをもらうための関数
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	WorldTransform worldTransform_;

	Model* model_ =  nullptr ;

	GameScene* gameScene_ = nullptr;

};

