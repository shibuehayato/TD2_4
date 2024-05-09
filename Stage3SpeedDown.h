#pragma once
#include"WorldTransform.h"
#include"Model.h"


class Stage3SpeedDown
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

	Vector3 GetPosition();

	Vector3 GetScale();

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	

	uint32_t textureHandle_ = 0u;
};

