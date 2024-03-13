#pragma once
#include"WorldTransform.h"
#include"Model.h"
class Pitfall
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_[2];

	Model* model_[2] = { nullptr };
};

