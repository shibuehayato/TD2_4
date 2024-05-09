#include "SpeedUp.h"
#include<cassert>
void SpeedUp::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	worldTransform_.translation_ = { 14.0f,0.0f,5.0f };

	textureHandle_ = TextureManager::Load("uvChecker.png");

	//worldTransform_.translation_ = { 0.0f,0.0f,-10.0f };
}

void SpeedUp::Update()
{
	worldTransform_.UpdateMatrix();
}

void SpeedUp::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 SpeedUp::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

Vector3 SpeedUp::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;
	return worldScale;

}