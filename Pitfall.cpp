#include "Pitfall.h"
#include<cassert>
void Pitfall::Initialize(Model* model)
{
	assert(model);

	/*model_ = model;
	worldTransform_.Initialize();*/

	for (int i = 0; i < 2; i++)
	{
		model_[i] = model;
		worldTransform_[i].Initialize();
	}
	worldTransform_[0].translation_ = { 14.0f,0.0f,23.0f };
	worldTransform_[1].translation_ = { -14.0f,0.0f,23.0f };

}

void Pitfall::Update()
{
	/*worldTransform_.UpdateMatrix();*/

	for (int i = 0; i < 2; i++)
	{
		worldTransform_[i].UpdateMatrix();
	}

}

void Pitfall::Draw(ViewProjection& viewProjection)
{
	/*model_->Draw(worldTransform_, viewProjection);*/

	for (int i = 0; i < 2; i++)
	{
		model_[i]->Draw(worldTransform_[i], viewProjection);
	}

}
