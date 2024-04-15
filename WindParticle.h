#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Model.h>
class WindParticle
{
public:
	void Initilize(Model* model);
	void Update();
	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

};

