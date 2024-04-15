#include "WindParticle.h"
#include <cassert>

void WindParticle::Initilize(Model* model)
{
	assert(model);
	model_ = model;


	worldTransform_.Initialize();

}

void WindParticle::Update()
{
	worldTransform_.UpdateMatrix();
}

void WindParticle::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
