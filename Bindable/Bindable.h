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
	virtual void upload(Graphics& gfx) = 0;
	virtual ~Buffer() = default;
};