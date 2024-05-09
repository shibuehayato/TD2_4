#include "Stage3RotateCannonBullet.h"
#include<cassert>
#include"Mymath.h"
void Stage3RotateCannonBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void Stage3RotateCannonBullet::Update()
{
	worldTransform_.UpdateMatrix();
	// 座標を移動させる(1フレーム分の移動量を足し込む)
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void Stage3RotateCannonBullet::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void Stage3RotateCannonBullet::OnCollision()
{
	isDead_ = true;
}

void Stage3RotateCannonBullet::OnCollision2()
{
	isDead_ = true;
}

Vector3 Stage3RotateCannonBullet::GetVelocity()
{
	Vector3 velocity;
	velocity.x = velocity_.x;
	velocity.y = velocity_.y;
	velocity.z = velocity_.z;
	return velocity;
}

Vector3 Stage3RotateCannonBullet::GetPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

Vector3 Stage3RotateCannonBullet::GetScale()
{
	Vector3 worldScale;
	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;
	return worldScale;
}
