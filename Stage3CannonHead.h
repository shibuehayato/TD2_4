#pragma once
#include "WorldTransform.h"
#include"Model.h"
#include"ImGuiManager.h"
#include"RotateCannonBullet.h"

class Stage3CannonHead
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = { nullptr };
	float rotation_ = 0.1f;
	bool isrotation_ = false;
};

