#include "RotateCannon.h"
#include<cassert>
#include"GameScene.h"
void RotateCannon::Initialize(Model* modelHead, Model* model)
{
	assert(modelHead);
	assert(model);
	model_[0] = modelHead;
	model_[1] = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = { -18.0f,0.0f,46.0f };
	worldTransformHead_.Initialize();
	worldTransformHead_.translation_ = { -17.0f,0.0f,46.0f };
	worldTransformHead_.scale_ = { 0.5f,1.0f,1.0f };
	worldTransformHead_.rotation_ = { 0.0f,-5.23f,0.0f };
	FiringInitialize();
}

void RotateCannon::Update()
{
	worldTransformHead_.UpdateMatrix();
	worldTransform_.UpdateMatrix();
	worldTransformHead_.rotation_.y += rotation_;
	if (isrotation_ == false)
	{
		rotation_ = 0.05f;
	}
	else if (isrotation_ == true)
	{
		rotation_ = -0.05f;
	}

	if (worldTransformHead_.rotation_.y >= -3.8f)
	{
		isrotation_ = true;
	}
	else if (worldTransformHead_.rotation_.y <= -5.8f)
	{
		isrotation_ = false;
	}

	FiringUpdate();

	/*ImGui::Begin("RotateCannon");
	ImGui::DragFloat3("rotation", &worldTransformHead_.rotation_.x, 0.1f);
	ImGui::End();*/

}

void RotateCannon::Draw(ViewProjection& viewProjection)
{
	model_[0]->Draw(worldTransformHead_, viewProjection);
	model_[1]->Draw(worldTransform_, viewProjection);
}

void RotateCannon::FiringInitialize()
{
	// 発射タイマーを初期化
	kFireTimer_ = kFireInterval_;
}

void RotateCannon::FiringUpdate()
{
	// 発射タイマーカウントダウン
	kFireTimer_--;
	// 指定時間に達した
	if (kFireTimer_ <= 0) {
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		kFireTimer_ = kFireInterval_;
	}
}

void RotateCannon::Fire()
{
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	velocity = TransformNormal(velocity, worldTransformHead_.matWorld_);

	velocity = Normalize(velocity);
	velocity = Multiply(kBulletSpeed, velocity);

	RotateCannonBullet* newRotateCannonBullet = new RotateCannonBullet();
	newRotateCannonBullet->Initialize(model_[1], worldTransformHead_.translation_, velocity);

	gameScene_->AddRotateCannonBullet(newRotateCannonBullet);
}

Vector3 RotateCannon::GetPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

Vector3 RotateCannon::GetScale()
{
	Vector3 worldScale;
	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;
	return worldScale;
}
