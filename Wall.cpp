#include "Wall.h"
#include <cassert>

void Wall::Initilize(Model* Wall)
{
	assert(Wall);

	worldTransform_.Initialize();
}

void Wall::Update()
{
	//worldTransform_.
}

void Wall::Draw(ViewProjection& viewProjection)
{
	Model_->Draw(worldTransform_, viewProjection);
}
