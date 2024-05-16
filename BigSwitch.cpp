#include "BigSwitch.h"
#include<cassert>

void BigSwitch::Initialize(Model* model, Model* modelbutton)
{
	assert(model);
	assert(modelbutton);
	model_ = model;
	modelbutton_ = modelbutton;
	worldTransformswitch_.Initialize();
	worldTransformbutton_.Initialize();


	worldTransformswitch_.translation_ = { 3.4f,1.0f,-50.0f };
	worldTransformswitch_.rotation_ = { 1.6f,3.15f,0.0f };
	worldTransformswitch_.scale_ = { 2.0f,2.0f,2.0f };
	worldTransformbutton_.translation_ = { 4.65f,1.0f,-50.0f };
	worldTransformbutton_.scale_ = { 2.0f,2.0f,2.0f };

	textureHandle_ = TextureManager::Load("switch_big2.png");

}

void BigSwitch::Update()
{
	worldTransformswitch_.UpdateMatrix();
	worldTransformbutton_.UpdateMatrix();

	if (isOncollision_ && worldTransformbutton_.scale_.x >= 1.6f)
	{
		worldTransformbutton_.scale_.x -= 0.01f;
	}


	if (worldTransformbutton_.scale_.x <= 1.6f)
	{
		isScale_ = true;
	}

	ImGui::Begin("BigSwitch");
	ImGui::DragFloat3("ButtonPosition", &worldTransformbutton_.translation_.x, 0.1f);
	ImGui::DragFloat3("ButtonRotation", &worldTransformswitch_.rotation_.x, 0.1f);
	ImGui::DragFloat3("ButtonScale", &worldTransformbutton_.scale_.x, 0.1f);
	ImGui::DragFloat3("SwitchPosition", &worldTransformswitch_.translation_.x, 0.1f);
	ImGui::End();

	
}

void BigSwitch::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransformswitch_, viewProjection,textureHandle_);
	modelbutton_->Draw(worldTransformbutton_, viewProjection);
}

void BigSwitch::OnCollision()
{
	isOncollision_ = true;

}

void BigSwitch::Position()
{

}

void BigSwitch::Reset()
{
	isOncollision_ = false;
	isScale_ = false;
	worldTransformbutton_.scale_ = { 2.0f,2.0f,2.0f };
}

Vector3 BigSwitch::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransformbutton_.matWorld_.m[3][0];
	worldPos.y = worldTransformbutton_.matWorld_.m[3][1];
	worldPos.z = worldTransformbutton_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 BigSwitch::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransformbutton_.scale_.x;
	worldScale.y = worldTransformbutton_.scale_.y;
	worldScale.z = worldTransformbutton_.scale_.z;

	return worldScale;
}