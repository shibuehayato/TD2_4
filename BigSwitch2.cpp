#include "BigSwitch2.h"
#include<cassert>

void BigSwitch2::Initialize(Model* model, Model* modelbutton)
{
	assert(model);
	assert(modelbutton);
	model_ = model;
	modelbutton_ = modelbutton;
	worldTransformswitch_.Initialize();
	worldTransformbutton_.Initialize();


	worldTransformswitch_.translation_ = { -17.5f,1.0f,-55.0f };
	worldTransformswitch_.rotation_ = { 0.0f,3.15f,0.0f };
	worldTransformswitch_.scale_ = { 2.0f,2.0f,2.0f };
	worldTransformbutton_.translation_ = { -16.25f,1.0f,-55.0f };
	worldTransformbutton_.scale_ = { 2.0f,2.0f,2.0f };

}

void BigSwitch2::Update()
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

	/*ImGui::Begin("NormalSwitch");
	ImGui::DragFloat3("ButtonPosition", &worldTransformbutton_.translation_.x, 0.1f);
	ImGui::DragFloat3("ButtonScale", &worldTransformbutton_.scale_.x, 0.1f);
	ImGui::DragFloat3("SwitchPosition", &worldTransformswitch_.translation_.x, 0.1f);
	ImGui::End();*/


}

void BigSwitch2::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransformswitch_, viewProjection);
	modelbutton_->Draw(worldTransformbutton_, viewProjection);
}

void BigSwitch2::OnCollision()
{
	isOncollision_ = true;

}

void BigSwitch2::Position()
{

}

void BigSwitch2::Reset()
{
	isOncollision_ = false;
	isScale_ = false;
	worldTransformbutton_.scale_ = { 2.0f,2.0f,2.0f };
}

Vector3 BigSwitch2::GetPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransformbutton_.matWorld_.m[3][0];
	worldPos.y = worldTransformbutton_.matWorld_.m[3][1];
	worldPos.z = worldTransformbutton_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 BigSwitch2::GetScale()
{
	Vector3 worldScale;

	worldScale.x = worldTransformbutton_.scale_.x;
	worldScale.y = worldTransformbutton_.scale_.y;
	worldScale.z = worldTransformbutton_.scale_.z;

	return worldScale;
}