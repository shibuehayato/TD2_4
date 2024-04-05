#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include"ImGuiManager.h"
class RightArrow
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);
	//他のクラスに渡す用の関数
	Vector3 GetVelocity();

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	Vector3 velocity_;
};

