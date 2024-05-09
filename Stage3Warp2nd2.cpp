#include "Stage3Warp2nd2.h"
#include<cassert>
void Stage3Warp2nd2::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 16.0f,-0.5f,-38.0f };
	worldTransform_.rotation_ = { 0.0f,3.2f,0.0f };
}

void Stage3Warp2nd2::Update()
{
	worldTransform_.UpdateMatrix();
}

void Stage3Warp2nd2::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void Stage3Warp2nd2::OnCollision()
{
	isOncollision_ = true;
}

void Stage3Warp2nd2::OnCollision2()
{
	isOncollision_ = false;
}

Vector3 Stage3Warp2nd2::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Stage3Warp2nd2::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;

	return worldScale;
}