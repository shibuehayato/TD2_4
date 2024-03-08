#include "Ball.h"
#include <cassert>

void Ball::Initialize(Model* ball, Model* cube)
{
	assert(ball);
	assert(cube);

	ballModel_ = ball;
	cubeModel_ = cube;

	worldTransform_.Initialize();
	ballWorldTransform_.Initialize();
	cubeWorldTransform_.Initialize();

	//ボールの親をにする
	ballWorldTransform_.parent_ = &worldTransform_;
	//箱の親をボールにする
	cubeWorldTransform_.parent_ = &ballWorldTransform_;

	ballWorldTransform_.translation_ = { 0,0,0 };

	cubeWorldTransform_.translation_ = { 0,0,0 };

}

void Ball::Update()
{
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
	ballWorldTransform_.UpdateMatrix();
	cubeWorldTransform_.UpdateMatrix();
}

void Ball::Draw(const ViewProjection& ViewProjection)
{
	// 3Dモデルを描画
	ballModel_->Draw(ballWorldTransform_, ViewProjection);
	cubeModel_->Draw(cubeWorldTransform_, ViewProjection);
}

void Ball::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

