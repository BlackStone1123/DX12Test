#include "Drawable.h"
#include <cassert>
#include <typeinfo>
#include "IndexBuffer.h"
#include "Bindable.h"
#include "Graphics.h"

void Drawable::Draw( Graphics& gfx )
{
	if (!buffers.empty())
	{
		for (auto buffer : buffers)
		{
			buffer->Upload(gfx);
		}
		buffers.clear();
	}

	for (auto& b : binds)
	{
		b->Bind(gfx);
	}

	auto heapSrc = GetHeapResource();
	gfx.DrawIndexed(pIndexBuffer->GetCount(), heapSrc.GetBindSlot(), heapSrc.GetResourceLocation());
}

void Drawable::AddBind( std::shared_ptr<Bindable> bind )
{
	// special case for index buffer
	if (typeid(*bind) == typeid(IndexBuffer))
	{
		assert("Binding multiple index buffers not allowed" && pIndexBuffer == nullptr);
		pIndexBuffer = &static_cast<IndexBuffer&>(*bind);
	}
	binds.push_back( std::move( bind ) );
}

void Drawable::AddResource(Resource* buffer)
{
	buffers.push_back(buffer);
}

Drawable::~Drawable() = default;