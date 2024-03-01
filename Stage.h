#pragma once
#include"WorldTransform.h"
#include"Model.h"
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
	WorldTransform worldTransform_[10];
	//モデルの変数
	Model* model_[10] = { nullptr };

};

