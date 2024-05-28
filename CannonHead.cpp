#include "CannonHead.h"
#include<cassert>
void CannonHead::Initialize(Model* model)
{
	assert(model);
	for (int i = 0; i < 3; i++)
	{
		model_[i] = model;

		worldTransform_[i].Initialize();
	}
	worldTransform_[0].translation_ = {-15,	1, -23};
	worldTransform_[0].rotation_ = {1.5f, -5.23f,	0};
	worldTransform_[1].translation_ = { 16,	1, -9 };
	worldTransform_[1].rotation_ = { 1.5f, -7.3f,0 };
	worldTransform_[2].translation_ = {-17.0f,1.0f,46.0f};
	worldTransform_[2].rotation_ = {1.5f,-5.23f,0.1f};
	worldTransform_[2].scale_ = { 1.0f,1.0f,1.0f };

}

void CannonHead::Update()
{
	worldTransform_[2].rotation_.y += rotation_;
	for (int i = 0; i < 3; i++)
	{
		worldTransform_[i].UpdateMatrix();
	}

	if (isrotation_ == false)
	{
		rotation_ = 0.05f;
	}
	else if (isrotation_ == true)
	{
		rotation_ = -0.05f;
	}

	if (worldTransform_[2].rotation_.y >= -3.8f)
	{
		isrotation_ = true;
	}
	else if (worldTransform_[2].rotation_.y <= -5.8f)
	{
		isrotation_ = false;
	}
	

	/*ImGui::Begin("Cannonhead");
	
	ImGui::DragFloat3("translation", &worldTransform_[2].translation_.x, 1.0f);
	ImGui::DragFloat3("rotation", &worldTransform_[2].rotation_.x, 0.1f);
	ImGui::DragFloat3("rotation", &worldTransform_[2].scale_.x, 0.1f);
	ImGui::End();*/
}

void CannonHead::Draw(ViewProjection& viewProjection)
{
	model_[0]->Draw(worldTransform_[0], viewProjection);
	model_[1]->Draw(worldTransform_[1], viewProjection);
	model_[2]->Draw(worldTransform_[2], viewProjection);
	
}
