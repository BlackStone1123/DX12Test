#include "Drawable.h"
#include <cassert>
#include <typeinfo>
#include "IndexBuffer.h"
#include "Bindable.h"
#include "Graphics.h"
#include "Material.h"

void Drawable::AttachMaterial(std::shared_ptr<Material> mat)
{
	AddBind(mat);
}

void Drawable::Draw( Graphics& gfx )
{
	if (!sources.empty())
	{
		for (auto buffer : sources)
		{
			buffer->Upload(gfx);
		}
	}

	for (auto& b : binds)
	{
		b->Bind(gfx);
	}

	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind( std::shared_ptr<Bindable> bind )
{
	// special case for index buffer
	if (typeid(*bind) == typeid(IndexBuffer))
	{
		assert("Binding multiple index buffers not allowed" && pIndexBuffer == nullptr);
		pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
		sources.push_back( pIndexBuffer);
	}
	else if (Resource* pSource = dynamic_cast<Resource*>(bind.get()))
	{
		sources.push_back(pSource);
	}

	binds.push_back( std::move( bind ) );
}

Drawable::~Drawable() = default;