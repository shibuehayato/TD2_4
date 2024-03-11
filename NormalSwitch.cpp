#include "NormalSwitch.h"
#include<cassert>
void NormalSwitch::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	textureHandle_ = TextureManager::Load("uvChecker.png");

	worldTransform_.translation_ = { -18.0f,0.0f,0.0f };
}

void NormalSwitch::Update()
{
	worldTransform_.UpdateMatrix();

	ImGui::Begin("NormalSwitch");
	ImGui::DragFloat3("position", &worldTransform_.translation_.x, 1.0f);
	ImGui::End();
}

void NormalSwitch::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection,textureHandle_);
}

Vector3 NormalSwitch::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 NormalSwitch::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;

	return worldScale;
}
