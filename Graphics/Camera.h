#pragma once
#include "Bindable.h"

class Camera : public ImguiItem
{
public:
	DirectX::XMMATRIX GetMatrix() const;
	virtual void Display() override;
	virtual void Reset()override;
private:
	float r = 20.0f;
	float theta = 0.0f;
	float phi = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
};

class Projector : public ImguiItem
{
public:
	Projector(UINT width, UINT height);
	virtual ~Projector() = default;

	DirectX::XMMATRIX GetMatrix() const;
	virtual void Display() override;
	virtual void Reset()override;

private:
	float mFov{ 45.0f };
	float mNear{ 0.1f };
	float mFar{ 500.0f };
	float mAspectRatio{ 1.0f };
	float mViewWidth{ 50.0f };
	float mViewHeight{ 50.0f };

	UINT mScreenWidth;
	UINT mScreenHeight;
	bool mPerspective{ true };
};