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
			buffer->upload(gfx);
		}
		buffers.clear();
	}

	for( auto& b : GetStaticBinds() )
	{
		b->Bind( gfx );
	}

	for (auto& b : binds)
	{
		b->Bind(gfx);
	}

	gfx.DrawIndexed( pIndexBuffer->GetCount() );
}

void Drawable::AddBind( std::unique_ptr<Bindable> bind )
{
	assert( "*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer) );
	binds.push_back( std::move( bind ) );
}

void Drawable::AddIndexBuffer( std::unique_ptr<IndexBuffer> ibuf )
{
	assert( "Attempting to add index buffer a second time" && pIndexBuffer == nullptr );
	pIndexBuffer = ibuf.get();
	binds.push_back( std::move( ibuf ) );
}

void Drawable::AddBuffer(Buffer* buffer)
{
	buffers.push_back(buffer);
}

Drawable::~Drawable() = default;