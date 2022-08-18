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
