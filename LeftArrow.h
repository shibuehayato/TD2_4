#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"ImGuiManager.h"
class LeftArrow
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

