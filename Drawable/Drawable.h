#pragma once
#include "Helpers.h"
#include <DirectXMath.h>
#include <vector>

class IndexBuffer;
class Bindable;
class Graphics;
class Buffer;

class Drawable
{
	template<class T>
	friend class DrawableBase;
public:
	Drawable() = default;
	Drawable( const Drawable& ) = delete;
	virtual ~Drawable();

	virtual DirectX::XMMATRIX GetTransformXM() const = 0;
	virtual void Update(Graphics& ,float dt) = 0;

	void Draw( Graphics& gfx );
	void AddBind( std::unique_ptr<Bindable> bind );
	void AddIndexBuffer( std::unique_ptr<IndexBuffer> ibuf );
	void AddBuffer(Buffer*);

private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const = 0;

private:
	const IndexBuffer* pIndexBuffer{ nullptr };
	std::vector<std::unique_ptr<Bindable>> binds;
	std::vector<Buffer*> buffers;
};