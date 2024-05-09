#include"Stage3Fire.h"
#include<cassert>
#include"GameScene.h"
void Stage3Fire::Initialize(Model* model, Vector3 position)
{
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("Cube.jpg");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
}

void Stage3Fire::Update()
{

	worldTransform_.UpdateMatrix();
}

void Stage3Fire::Draw(ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection,textureHandle_);
}

Vector3 Stage3Fire::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Stage3Fire::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;

	return worldScale;
}