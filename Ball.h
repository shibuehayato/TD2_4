#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include "Model.h"

class Ball
{
public:
	void Initialize(Model* ball, Model* cube);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	//親となるワールドトランスフォーム
	void SetParent(const WorldTransform* parent);

private:
	WorldTransform worldTransform_;
	WorldTransform ballWorldTransform_;
	WorldTransform cubeWorldTransform_;

	Model* ballModel_=nullptr;
	Model* cubeModel_=nullptr;

	enum {
		kModelIndexBall,
		kModelIndexCube,
	};
};

