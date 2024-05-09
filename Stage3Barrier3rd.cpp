#include "Stage3Barrier3rd.h"

void Stage3Barrier3rd::Initialize(Model* model, Vector3 position)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	isDead_ = false;
}

void Stage3Barrier3rd::Update()
{
	worldTransform_.UpdateMatrix();
	//ImGui::Begin("Barrier3");
	//ImGui::DragFloat3("position", &worldTransform_.translation_.x, 1.0f);
	//ImGui::End();
}

void Stage3Barrier3rd::Draw(ViewProjection& viewProjection)
{
	if (isDead_ == false)
	{
		model_->Draw(worldTransform_, viewProjection);
	}
}

void Stage3Barrier3rd::OnCollision()
{
	isDead_ = true;
}

Vector3 Stage3Barrier3rd::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Stage3Barrier3rd::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;

	return worldScale;
}
