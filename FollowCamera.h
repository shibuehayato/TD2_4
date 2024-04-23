#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class FollowCamera
{
public:
	void Initialize();
	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }

	const ViewProjection& GetViewProjection() { return viewProjection_; }

private:
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 追従対象
	const WorldTransform* target_ = nullptr;

	Vector3 offset = { 0.0f,19.0f,-25.0f };
	Vector3 rotation = { 0.7f,0.0f,0.0f };
};