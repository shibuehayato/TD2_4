#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"Input.h"
#include"ImGuiManager.h"

class GameScene;

class Stage
{
public:
	//初期化
	void Initialize(Model* model,Vector3 position);

	//更新
	void Update();

	//描画
	void Draw(ViewProjection& viewProjection);

	//ゲームシーンからアドレスをもらうための関数
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//ワールド座標を取得
	Vector3 GetPosition();

	//ワールドサイズを取得
	Vector3 GetScale();

private:
	//モデルの大きさや回転や座標を入れる変数
	WorldTransform worldTransform_;

	//WorldTransform worldTransform2_;

	//モデルの変数
	Model* model_ = nullptr;

	//Model* model2_ = nullptr;

	Input* input_ = nullptr;

	uint32_t textureHandle_;

	GameScene* gameScene_ = nullptr;

};

