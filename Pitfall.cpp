#include "Pitfall.h"
#include<cassert>
#include"GameScene.h"
void Pitfall::Initialize(Model* model, Vector3 position)
{
	assert(model);

	model_ = model;
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	/*for (int i = 0; i < 2; i++)
	{
		model_[i] = model;
		worldTransform_[i].Initialize();
	}
	worldTransform_[0].translation_ = { 14.0f,0.0f,23.0f };
	worldTransform_[1].translation_ = { -14.0f,0.0f,23.0f };*/

}

void Pitfall::Update()
{
	worldTransform_.UpdateMatrix();

	/*for (int i = 0; i < 2; i++)
	{
		worldTransform_[i].UpdateMatrix();
	}*/

}

void Pitfall::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);

	/*for (int i = 0; i < 2; i++)
	{
		model_[i]->Draw(worldTransform_[i], viewProjection);
	}*/

}

Vector3 Pitfall::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Pitfall::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.z;
	worldScale.z = worldTransform_.scale_.z;

	return worldScale;
}
