#include "RightArrow.h"
#include<cassert>
#include"Player.h"
#include <Mymath.h>

void RightArrow::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();

	
	worldTransform_.translation_ = { 6.0f,0.0f,34.0f };
	worldTransform_.rotation_ = { 0.0f,-1.6f,0.0f };
	velocity_ = { 1.0f,1.0f,1.0f };

	ArrowRot = { 1.0f,1.0f,1.0f };
}

void RightArrow::Update()
{
	worldTransform_.UpdateMatrix();

	
		ArrowRot = { -1.0f,0.0f,0.0f };

	//	ImGui::Begin("right");
	//ImGui::DragFloat3("rightrot", &ArrowRot.x, 0.1f);
	//ImGui::End();

}

void RightArrow::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void RightArrow::OnCollision(Player* player)
{
	//速度調整
	Vector3 speedScale = { 2.0f,0,2.0f };

	Vector3 arrowSpeed = Transform(ArrowRot, speedScale);

	//プレイヤーに速度あげる　多分入ってる
	player->SetKeepMove(arrowSpeed);

	//playerの当たり判定ここで
	player->ArrowOnCollision();
}

Vector3 RightArrow::GetVelocity()
{
	Vector3 velocity;

	velocity.x = velocity_.x;
	velocity.y = velocity_.y;
	velocity.z = velocity_.z;
	return velocity;
}
