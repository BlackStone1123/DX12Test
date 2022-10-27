#pragma once
#include "Helpers.h"
#include <DirectXMath.h>
#include <vector>
#include "HeapAllocation.h"

class IndexBuffer;
class Bindable;
class Graphics;
class Resource;
class Material;

class Drawable
{
public:
	using BindableList = std::vector<std::shared_ptr<Bindable>>;
	using ResourceList = std::vector<Resource*>;

	Drawable() = default;
	Drawable( const Drawable& ) = delete;
	virtual ~Drawable();

	virtual DirectX::XMMATRIX GetTransformXM() const = 0;
	virtual void Update(float dt) = 0;

	void AttachMaterial(std::shared_ptr<Material> mat);
	void Draw( Graphics& gfx );

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

private:

	IndexBuffer* pIndexBuffer{ nullptr };
	BindableList binds;
	ResourceList sources;
};