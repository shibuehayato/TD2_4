#include "Cannon.h"
#include<cassert>
#include"GameScene.h"
void Cannon::Initialize(Model* modelHead, Model* model, Vector3 position, Vector3 HeadPosition, Vector3 rotation, Model* modelbullet)
{
	assert(modelHead);
	assert(model);
	model_[0] = modelHead;
	model_[1] = model;
	model_[2] = modelbullet;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	//worldTransform_.translation_ = { -18.0f,0.0f,-24.0f };
	//worldTransform_.translation_ = { -18.0f,0.0f,-10.0f };
	worldTransformHead_.Initialize();
	worldTransformHead_.translation_ = HeadPosition;
	//worldTransformHead_.translation_ = { -17,0.0f,-24.0f };
	worldTransformHead_.scale_ = { 0.5f,1.0f,1.0f };
	worldTransformHead_.rotation_ = rotation;

	
	//worldTransformHead_.rotation_ = { 0.0f,-8.2f,0.0f };
	//worldTransformHead_.rotation_ = { 0.0f,-7.3f,0.0f };
	//worldTransformHead_.translation_ = { -17.0f,0.0f,-10.0f };

	FiringInitialize();

}


void Cannon::Update()
{
	worldTransformHead_.UpdateMatrix();
	worldTransform_.UpdateMatrix();

	FiringUpdate();

	/*ImGui::Begin("Cannon");
	ImGui::DragFloat3("translationHead2", &worldTransformHead_.translation_.x, 1.0f);
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 1.0f);
	ImGui::DragFloat3("translation", &worldTransform_.rotation_.x, 1.0f);
	ImGui::DragFloat3("Head2Rotation", &worldTransformHead2_.rotation_.x, 0.01f);
	ImGui::End();*/
}

void Cannon::Draw(ViewProjection& viewProjection)
{
	//model_[0]->Draw(worldTransformHead_, viewProjection);
	model_[1]->Draw(worldTransform_, viewProjection);
}

void Cannon::FiringInitialize()
{
	// 発射タイマーを初期化
	kFireTimer_ = kFireInterval_;
}

void Cannon::FiringUpdate()
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

void Cannon::Fire()
{
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	velocity = TransformNormal(velocity, worldTransformHead_.matWorld_);

	velocity = Normalize(velocity);
	velocity = Multiply(kBulletSpeed, velocity);
	
	Cannonbullet* newCannonBullet = new Cannonbullet();
	newCannonBullet->Initialize(model_[2], worldTransformHead_.translation_, velocity);

	gameScene_->AddCannonBullet(newCannonBullet);
}

Vector3 Cannon::GetPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

Vector3 Cannon::GetScale()
{
	Vector3 worldScale;
	worldScale.x = worldTransform_.scale_.x;
	worldScale.y = worldTransform_.scale_.y;
	worldScale.z = worldTransform_.scale_.z;
	return worldScale;
}
