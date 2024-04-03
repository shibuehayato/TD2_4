#include "SmallSwitch.h"
#include<cassert>

void SmallSwitch::Initialize(Model* model, Model* modelbutton)
{
	assert(model);
	assert(modelbutton);
	model_ = model;
	modelbutton_ = modelbutton;
	worldTransformswitch_.Initialize();
	worldTransformbutton_.Initialize();
	

	worldTransformswitch_.translation_ = { -20.25f,0.0f,49.0f };
	worldTransformswitch_.rotation_ = { 0.0f,3.15f,0.0f };
	worldTransformbutton_.translation_ = { -19.0f,0.0f,49.0f };
}

void SmallSwitch::Update()
{
	worldTransformswitch_.UpdateMatrix();
	worldTransformbutton_.UpdateMatrix();

	if (isOncollision_ && worldTransformbutton_.scale_.x >= 0.6f)
	{
		worldTransformbutton_.scale_.x -= 0.01f;
	}
	
	if (isOncollision_ && worldTransformswitch_.translation_.x >= -18.5f)
	{
		worldTransformswitch_.translation_.x -= 0.01f;
	}
	if (worldTransformbutton_.scale_.x <= 0.6f)
	{
		isScale_ = true;
	}
	ImGui::Begin("SmallSwitch");
	ImGui::DragFloat3("ButtonPosition", &worldTransformbutton_.translation_.x, 0.1f);
	ImGui::DragFloat3("ButtonScale", &worldTransformbutton_.scale_.x,0.1f);
	ImGui::DragFloat3("SwitchPosition", &worldTransformswitch_.translation_.x, 0.1f);
	ImGui::End();
}

void SmallSwitch::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransformswitch_, viewProjection);
	modelbutton_->Draw(worldTransformbutton_, viewProjection);
}

void SmallSwitch::OnCollision()
{
	isOncollision_ = true;
	
}

Vector3 SmallSwitch::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransformbutton_.matWorld_.m[3][0];
	worldPos.y = worldTransformbutton_.matWorld_.m[3][1];
	worldPos.z = worldTransformbutton_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 SmallSwitch::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransformbutton_.scale_.x;
	worldScale.y = worldTransformbutton_.scale_.y;
	worldScale.z = worldTransformbutton_.scale_.z;

	return worldScale;
}
