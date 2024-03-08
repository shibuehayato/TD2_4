#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include "Model.h"

class Ball
{
public:
	void Initialize(Model* ball);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	
private:
	WorldTransform ballWorldTransform_;

	Model* ballModel_=nullptr;

};

