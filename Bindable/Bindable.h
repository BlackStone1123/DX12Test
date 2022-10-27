#pragma once
#include "CommonHeaders.h"
#include "Helpers.h"
#include "HeapAllocation.h"

class Graphics;
class Bindable
{
public:
	virtual void Bind( Graphics& gfx ) = 0;
	virtual ~Bindable() = default;
};

class Resource
{
public:
	virtual void Upload(Graphics& gfx) = 0;
	virtual ~Resource() = default;

protected:
	bool mUpLoaded{ false };
};

class ImguiItem
{
public:
	virtual void Display() = 0;
	virtual void Reset() {}
	virtual ~ImguiItem() = default;
};