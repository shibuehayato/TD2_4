#include "Stage3Barrier2nd.h"

void Stage3Barrier2nd::Initialize(Model* model, Vector3 position)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	isDead_ = false;
}

void Stage3Barrier2nd::Update()
{
	worldTransform_.UpdateMatrix();
	/*ImGui::Begin("Barrier3");
	ImGui::DragFloat3("position", &worldTransform_.translation_.x, 1.0f);
	ImGui::End();*/
	if (isOnCollision_ == true)
	{
		worldTransform_.translation_.y++;
	}
	if (worldTransform_.translation_.y >= 10.0f)
	{
		isDead_ = true;
		isOnCollision_ = false;
	}
}

void Stage3Barrier2nd::Draw(ViewProjection& viewProjection)
{
	if (isDead_ == false)
	{
		model_->Draw(worldTransform_, viewProjection);
	}
}

void Stage3Barrier2nd::OnCollision()
{
	isOnCollision_ = true;
}

void Stage3Barrier2nd::Reset()
{
	isDead_ = false;
	isOnCollision_ = false;
	worldTransform_.translation_.y = 0;
}

Vector3 Stage3Barrier2nd::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Stage3Barrier2nd::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;

	return worldScale;
}
