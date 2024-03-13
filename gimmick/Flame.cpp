#include "Flame.h"
#include<cassert>
#include"GameScene.h"
void Flame::Initialize(Model* model,Vector3 position)
{
	assert(model);

	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	/*for (int i = 0; i < 3; i++)
	{
		model_[i] = model;
		worldTransform_[i].Initialize();
	}

	worldTransform_[0].translation_ = { -18.0f,0.0f,-25.0f };
	worldTransform_[1].translation_ = { 18.0f,0.0f,-5.0f };
	worldTransform_[2].translation_ = { 18.0f,0.0f,35.0f };*/

}

void Flame::Update()
{

	worldTransform_.UpdateMatrix();

	/*for (int i = 0; i < 3; i++)
	{
		worldTransform_[i].UpdateMatrix();
	}*/

	/*ImGui::Begin("Flame");

    ImGui::DragFloat3("Position", &worldTransform_[0].translation_.x, 1.0f);
	ImGui::DragFloat3("Position", &worldTransform_[1].translation_.x, 1.0f);
	ImGui::DragFloat3("Position", &worldTransform_[2].translation_.x, 1.0f);

    ImGui::End();*/

}

void Flame::Draw(ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

	/*for (int i = 0; i < 3; i++)
	{
		model_[i]->Draw(worldTransform_[i], viewProjection);
	}*/
}

Vector3 Flame::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Flame::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;

	return worldScale;
}
