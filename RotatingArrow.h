#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"ImGuiManager.h"
class RotatingArrow
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
};

