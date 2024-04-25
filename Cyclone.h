#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Model.h>
class Cyclone
{
public:
	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection& viewProjection);

	bool IsDead();


private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
};

