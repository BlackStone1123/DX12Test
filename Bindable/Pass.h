#pragma once
#include "CommonHeaders.h"
#include "Helpers.h"

class Drawable;
class Bindable;
class Graphics;
class PointLight;

class Pass 
{
public:
	using DrawableList = std::vector<Drawable*>;
	using BindableList = std::vector<std::unique_ptr<Bindable>>;

	virtual ~Pass() = default;
	virtual void Execute(Graphics& gfx);
	virtual void AttachDrawable(Drawable*);

protected:
	DrawableList mDrawables;
	BindableList mBindables;
};

class SolidColorPass : public Pass
{
public:
	SolidColorPass(Graphics& gfx, const DirectX::XMFLOAT3& color);
	virtual void AttachDrawable(Drawable*) override;
};

class BorderPass : public Pass
{
public:
	BorderPass(Graphics& gfx, const DirectX::XMFLOAT3& color);
	virtual void AttachDrawable(Drawable*) override;
};

class PhongShaderPass : public Pass
{
public:
	PhongShaderPass(Graphics& gfx);
	void SetPointLight(PointLight*);

	virtual void Execute(Graphics& gfx) override;
	virtual void AttachDrawable(Drawable*) override;

private:
	PointLight* mPt {nullptr};
};