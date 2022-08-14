#pragma once
#include "DrawableBase.h"

class Box : public DrawableBase<Box>
{
public:
	Box( Graphics& gfx,std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist );
	void Update(Graphics&, float dt ) override;
	DirectX::XMMATRIX GetTransformXM() const override;
private:
	// positional
	float x{0.0f};
	float z{0.0f};
	float y{ 0.0f };
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
	float dz{1.0f};
	float dx{0.5f};
	float dy{0.0f};
};