#pragma once
#include "WorldTransform.h"
#include"Model.h"
#include"ImGuiManager.h"
class CannonHead
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_[3];
	Model* model_[3] = { nullptr };
	float rotation_ = 0.1f;
	bool isrotation_ = false;
};

