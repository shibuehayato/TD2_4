#include "DownArrow.h"
#include<cassert>
#include"Player.h"
#include <Mymath.h>

void DownArrow::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 6.0f,0.0f,50.0f };

	velocity_ = { 1.0f,1.0f,1.0f };

	ArrowRot = { 1.0f,1.0f,1.0f };
}

void DownArrow::Update()
{
	worldTransform_.UpdateMatrix();

	ArrowRot = { 0.0f,0.0f,1.0f };
	
}

void DownArrow::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void DownArrow::OnCollision(Player* player)
{
	//速度調整
	Vector3 speedScale = { 2.0f,0,2.0f };

	Vector3 arrowSpeed = Transform(ArrowRot, speedScale);

	//プレイヤーに速度あげる　多分入ってる
	player->SetKeepMove(arrowSpeed);

	//playerの当たり判定ここで
	player->ArrowOnCollision();
}

Vector3 DownArrow::GetVelocity()
{
	Vector3 velocity;

	velocity.x = velocity_.x;
	velocity.y = velocity_.y;
	velocity.z = velocity_.z;
	return velocity;
}
