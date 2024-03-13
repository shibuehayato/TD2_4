#include "DebugPlayer.h"
#include <cassert>
#include <ImGuiManager.h>

void DebugPlayer::Initialize(Model* Wall)
{
	input_ = Input::GetInstance();

	assert(Wall);
	Model_ = Wall;

	worldTransform_.Initialize();

	worldTransform_.translation_ = { -20,0,0 };
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.rotation_ = { 0,0,0 };

	speed_={ 0.5f,0.5f,0.0f };
}

void DebugPlayer::Update()
{

	if (input_->PushKey(DIK_A)) {
		worldTransform_.translation_.x -= speed_.x;
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.translation_.x += speed_.x;
	}
	if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.y -= speed_.y;
	}
	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.y += speed_.y;
	}

	if (input_->TriggerKey(DIK_SPACE)) {
		speed_.x *=-1;
		speed_.y *=-1;
	}

	//if (input_->PushKey(DIK_A)) {
	//	worldTransform_.translation_.x -= speed_.x*10;
	//}

	//worldTransform_.translation_.x += speed_.x;

	ImGui::Begin("window");
	ImGui::DragFloat3("Playertransrate", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("Playerangle", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("Playersize", &worldTransform_.scale_.x, 0.01f);
	ImGui::End();

	worldTransform_.UpdateMatrix();
}

void DebugPlayer::Draw(ViewProjection& viewProjection){
	Model_->Draw(worldTransform_, viewProjection);
}

void DebugPlayer::WallOnCollision()
{
	speed_.x *= -1;
	speed_.y *= -1;
}
