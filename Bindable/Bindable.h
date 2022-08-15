#pragma once
#include "CommonHeaders.h"
#include "Helpers.h"

class Graphics;
class Bindable
{
public:
	virtual void Bind( Graphics& gfx ) = 0;
	virtual ~Bindable() = default;
};

class Buffer
{
public:
	virtual void Upload(Graphics& gfx) = 0;
	virtual ~Buffer() = default;
};

class ImguiItem
{
public:
	virtual void Display() = 0;
	virtual void Reset() = 0;
	virtual ~ImguiItem() = default;
};