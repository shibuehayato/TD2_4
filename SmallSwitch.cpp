#include "SmallSwitch.h"
#include<cassert>
void SmallSwitch::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	textureHandle_ = TextureManager::Load("sample.png");

	worldTransform_.translation_ = { -18.0f,0.0f,50.0f };

}

void SmallSwitch::Update()
{
	worldTransform_.UpdateMatrix();

	

}

void SmallSwitch::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection,textureHandle_);
}

Vector3 SmallSwitch::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 SmallSwitch::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;

	return worldScale;
}
