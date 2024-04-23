#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Model.h>
class WindParticle
{
public:
	void Initialize(Model* model,float startX, float startY,float startZ);
	void Update();
	void Draw(ViewProjection& viewProjection);

	bool IsDead();


private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	// デスフラグ
	bool isDead_ = false;
	// デスタイマー
	float deathTimer_ = 23;
};

