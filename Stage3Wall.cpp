#include "Stage3Wall.h"
#include<cassert>
#include"GameScene.h"
void Stage3Wall::Initialize(Model* model, Vector3 position)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void Stage3Wall::Update()
{
	worldTransform_.UpdateMatrix();
}

void Stage3Wall::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 Stage3Wall::GetPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

Vector3 Stage3Wall::GetScale()
{
	Vector3 worldScale;
	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;
	return worldScale;
}
