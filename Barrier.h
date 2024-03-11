#pragma once
#include"WorldTransform.h"
#include"Model.h"
class Barrier
{
public:
	void Initialize();

	void Update();

	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

};

