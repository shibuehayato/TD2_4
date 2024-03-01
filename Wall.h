#pragma once
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <Model.h>
class Wall
{
public:

	void Initilize(Model*Wall);

	void Update();

	void Draw(ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* Model_ = nullptr;
};

