#include "Stage.h"
#include<cassert>

void Stage::Initialize(Model* model)
{
	//問題ないか変数をチェック
	assert(model);
	//モデルの代入
	model_[0] = model;
	//モデルの初期化と座標を指定
	worldTransform_[0].Initialize();
	worldTransform_[0].translation_ = {4.0f,0.0f,0.0f};
	//-------------------------//



}

void Stage::Update()
{
	//worldTransform_[0].UpdateMatrix();
}

void Stage::Draw(ViewProjection& viewProjection)
{
	//モデルの描画
	model_[0]->Draw(worldTransform_[0], viewProjection);
}
