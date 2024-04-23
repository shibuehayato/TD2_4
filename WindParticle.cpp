#include "WindParticle.h"
#include <cassert>

void WindParticle::Initialize(Model* model, float startX, float startY, float startZ)
{
	assert(model);
	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.translation_.x = startX;
	worldTransform_.translation_.y = startY;
	worldTransform_.translation_.z = startZ;

	isDead_ = false;

	deathTimer_ = 8;
}

void WindParticle::Update()
{
	worldTransform_.translation_.x -= 0.25f;

	deathTimer_ -= 0.1f;
	if (deathTimer_ <= 0) {
		isDead_ = true;
		deathTimer_ = 50;
	}

	worldTransform_.UpdateMatrix();
}

void WindParticle::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

bool WindParticle::IsDead()
{
 return isDead_;
}
