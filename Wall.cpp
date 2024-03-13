#include "Wall.h"
#include <cassert>
#include <ImGuiManager.h>

void Wall::Initialize(Model* Wall)
{
	assert(Wall);
	Model_ = Wall;

	worldTransform_.Initialize();

	worldTransform_.translation_={ 0,0,0 };
	worldTransform_.scale_={1,1,1 };
	worldTransform_.rotation_ = { 0,0,0 };
}

void Wall::Update()
{
	worldTransform_.UpdateMatrix();

	ImGui::Begin("window");
	ImGui::DragFloat3("transrate", &worldTransform_.translation_.x,0.1f);
	ImGui::DragFloat3("angle", &worldTransform_.rotation_.x,0.1f);
	ImGui::DragFloat3("size", &worldTransform_.scale_.x,0.1f);
	ImGui::End();
}

void Wall::Draw(ViewProjection& viewProjection)
{
	Model_->Draw(worldTransform_, viewProjection);
}
