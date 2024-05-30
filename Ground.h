#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"ImGuiManager.h"

class GameScene;

class Ground
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

	//ゲームシーンからアドレスをもらうための関数
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	WorldTransform worldTransform_;

	Model* model_=nullptr;

	GameScene* gameScene_ = nullptr;
};

