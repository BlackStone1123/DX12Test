#pragma once
#include "Helpers.h"
#include <DirectXMath.h>
#include <vector>
#include "HeapAllocation.h"

class IndexBuffer;
class Bindable;
class Graphics;
class Resource;

class Drawable
{
	template<class T>
	friend class DrawableBase;
public:
	using BindableList = std::vector<std::shared_ptr<Bindable>>;
	using BufferList = std::vector<Resource*>;

	Drawable() = default;
	Drawable( const Drawable& ) = delete;
	virtual ~Drawable();

	virtual DirectX::XMMATRIX GetTransformXM() const = 0;
	virtual HeapResource GetHeapResource() const = 0;
	virtual void Update(float dt) = 0;

	void Draw( Graphics& gfx );
	virtual void DrawOutline(Graphics& gfx) {}

	template<class T>
	T* QueryBindable() noexcept
	{
		for (auto& pb : binds)
		{
			if (auto pt = dynamic_cast<T*>(pb.get()))
			{
				return pt;
			}
		}
		return nullptr;
	}

protected:
	void AddBind(std::shared_ptr<Bindable> bind);
	void AddResource(Resource*);

private:
	virtual const BindableList& GetStaticBinds() const = 0;

private:
	const IndexBuffer* pIndexBuffer{ nullptr };
	BindableList binds;
	BufferList buffers;
};