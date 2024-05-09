#include "Stage3Warp4th.h"
#include<cassert>
void Stage3Warp4th::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 14.0f,-0.5f,-27.0f };
	worldTransform_.rotation_ = { 0.0f,3.2f,0.0f };
}

void Stage3Warp4th::Update()
{
	worldTransform_.UpdateMatrix();
}

void Stage3Warp4th::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void Stage3Warp4th::OnCollision()
{
	isOncollision_ = true;
}

void Stage3Warp4th::OnCollision2()
{
	isOncollision_ = false;
}

Vector3 Stage3Warp4th::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Stage3Warp4th::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;

	return worldScale;
}
