#pragma once
#include "Bindable.h"

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