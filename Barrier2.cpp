#include "Barrier2.h"
#include<cassert>
#include"GameScene.h"
void Barrier2::Initialize(Model* model, Vector3 position)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	/*for (int i = 0; i < 50; i++)
	{
		model_[i] = model;
		worldTransform_[i].Initialize();
	}

	worldTransform_[0].translation_ = { 7.5f,0.0f,6.5f };
	worldTransform_[1].translation_ = { 5.5f,0.0f,6.5f };
	worldTransform_[2].translation_ = { 9.5f,0.0f,6.5f };
	worldTransform_[3].translation_ = { 11.5f,0.0f,6.5f };
	worldTransform_[4].translation_ = { 11.5f,0.0f,4.5f };
	worldTransform_[5].translation_ = { 11.5f,0.0f,2.5f };
	worldTransform_[6].translation_ = { 11.5f,0.0f,0.5f };
	worldTransform_[7].translation_ = { 9.5f,0.0f,0.5f };
	worldTransform_[8].translation_ = { 7.5f,0.0f,0.5f };
	worldTransform_[9].translation_ = { 5.5f,0.0f,0.5f };
	worldTransform_[10].translation_ = { 3.5f,0.0f,0.5f };
	worldTransform_[11].translation_ = { 3.5f,0.0f,2.5f };
	worldTransform_[12].translation_ = { 3.5f,0.0f,4.5f };
	worldTransform_[13].translation_ = { 3.5f,0.0f,6.5f };*/

}

void Barrier2::Update()
{
	worldTransform_.UpdateMatrix();
	/*for (int i = 0; i < 50; i++)
	{
		worldTransform_[i].UpdateMatrix();
	}*/
	ImGui::Begin("Barrier2");
	ImGui::DragFloat3("position", &worldTransform_.translation_.x, 1.0f);
	ImGui::End();
}

void Barrier2::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
	/*for (int i = 0; i < 50; i++)
	{
		model_[i]->Draw(worldTransform_[i], viewProjection);
	}*/
}

Vector3 Barrier2::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Barrier2::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;

	return worldScale;
}
