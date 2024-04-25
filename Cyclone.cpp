#include "Cyclone.h"
#include <cassert>
#include <imgui.h>

void Cyclone::Initialize(Model* model)
{
	assert(model);
	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.translation_ = { 17,2,-25 };
	worldTransform_.rotation_ = { 1.56f,0,4.7f };

}

void Cyclone::Update()
{
	worldTransform_.rotation_.y += 0.1f;

	if (worldTransform_.rotation_.y >= 6 ){
		worldTransform_.rotation_.y=0;
	}

	worldTransform_.UpdateMatrix();

	ImGui::Begin("Cyclone");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x);
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x);
	ImGui::End();
}

void Cyclone::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
