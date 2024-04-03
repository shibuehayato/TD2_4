#include "NormalSwitch.h"
#include<cassert>
void NormalSwitch::Initialize(Model* model, Model* modelbutton)
{
	assert(model);
	assert(modelbutton);
	model_ = model;
	modelbutton_ = modelbutton;
	worldTransform_.Initialize();
	worldTransformbutton_.Initialize();
	

	worldTransform_.translation_ = { -20.25f,0.0f,1.0f };
	worldTransform_.rotation_ = { 0.0f,3.15f,0.0f };
	worldTransformbutton_.translation_ = { -19.5f,0.0f,1.0f };
}

void NormalSwitch::Update()
{
	worldTransform_.UpdateMatrix();
	worldTransformbutton_.UpdateMatrix();

	if (isOncollision_ && worldTransformbutton_.scale_.x >= 0.6f)
	{
		worldTransformbutton_.scale_.x -= 0.01f;
	}

	
	if (worldTransformbutton_.scale_.x <= 0.6f)
	{
		isScale_ = true;
	}
}

void NormalSwitch::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
	modelbutton_->Draw(worldTransformbutton_, viewProjection);
}

void NormalSwitch::OnCollision()
{
	isOncollision_ = true;
	
}

Vector3 NormalSwitch::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransformbutton_.matWorld_.m[3][0];
	worldPos.y = worldTransformbutton_.matWorld_.m[3][1];
	worldPos.z = worldTransformbutton_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 NormalSwitch::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransformbutton_.scale_.x;
	worldScale.y = worldTransformbutton_.scale_.y;
	worldScale.z = worldTransformbutton_.scale_.z;

	return worldScale;
}
