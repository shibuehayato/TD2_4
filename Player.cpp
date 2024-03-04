#include "Player.h"
#include <cassert>

void Player::Initialize(Model* head)
{
	assert(head);
	HeadModel_ = head;

	worldTransformHead_.translation_ = { 0,0,-15.0f };
	worldTransformHead_.Initialize();
}

void Player::Update() {

	// 行列を定数バッファに転送
	worldTransformHead_.UpdateMatrix();
}

void Player::Draw(ViewProjection viewProjection) { 
	// 3Dモデル描画
	HeadModel_->Draw(worldTransformHead_, viewProjection);
}