#include "SpeedDown.h"
#include"GameScene.h"
#include<cassert>
void SpeedDown::Initialize(Model* model, Vector3 position)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	textureHandle_ = TextureManager::Load("uvChecker.png");
	worldTransform_.rotation_ = { 0.0f,3.12f,0.0f };
	//worldTransform_.translation_ = { 0.0f,0.0f,-10.0f };
}

void SpeedDown::Update()
{
	worldTransform_.UpdateMatrix();
	
}

void SpeedDown::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 SpeedDown::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

Vector3 SpeedDown::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;
	return worldScale;

}
