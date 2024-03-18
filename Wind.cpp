#include "Wind.h"
#include<cassert>
#include"GameScene.h"
void Wind::Initialize(Model* model, Vector3 position)
{
	assert(model);
	model_ = model;

	worldTransform_.translation_ = position;

	worldTransform_.Initialize();

	/*for (int i = 0; i < 3; i++)
	{
		model_[i] = model;
		worldTransform_[i].Initialize();
	}

	worldTransform_[0].translation_ = { 18.0f,0.0f,-25.0f };
	worldTransform_[1].translation_ = { 18.0f,0.0f,-23.0f };
	worldTransform_[2].translation_ = { 18.0f,0.0f,-27.0f };*/

}

void Wind::Update()
{
	worldTransform_.UpdateMatrix();
	/*for (int i = 0; i < 3; i++)
	{
		worldTransform_[i].UpdateMatrix();
	}*/

}

void Wind::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
	/*for (int i = 0; i < 3; i++)
	{
		model_[i]->Draw(worldTransform_[i], viewProjection);
	}*/


}
