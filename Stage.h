#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"Input.h"
class Stage
{
public:
	//初期化
	void Initialize(Model* model);

	//更新
	void Update();

	//描画
	void Draw(ViewProjection& viewProjection);

private:
	//モデルの大きさや回転や座標を入れる変数
	WorldTransform worldTransform_[281];
	//モデルの変数
	Model* model_[281] = { nullptr};

	Input* input_ = nullptr;

};

