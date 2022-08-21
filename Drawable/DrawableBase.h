#pragma once
#include "Drawable.h"
#include "Bindable.h"

template<class T>
class DrawableBase : public Drawable
{
public:
	bool IsStaticInitialized() const
	{
		return !staticBinds.empty();
	}
	void AddStaticBind( std::unique_ptr<Bindable> bind )
	{
		if (auto buffer = dynamic_cast<Resource*>(bind.get()))
		{
			AddResource(buffer);
		}
		staticBinds.push_back( std::move( bind ) );
	}
	void AddStaticIndexBuffer( std::unique_ptr<IndexBuffer> ibuf )
	{
		pIndexBuffer = ibuf.get();
		AddResource(ibuf.get());
		staticBinds.push_back( std::move( ibuf ) );
	}
	void SetIndexFromStatic()
	{
		for( const auto& b : staticBinds )
		{
			if( const auto p = dynamic_cast<IndexBuffer*>(b.get()) )
			{
				pIndexBuffer = p;
				return;
			}
		}
	}
	void SetHeapResource(const HeapResource& location)
	{
		heapResource = location;
	}
private:
	virtual const BindableList& GetStaticBinds() const override
	{
		return staticBinds;
	}

	virtual HeapResource GetHeapResource() const override
	{
		return heapResource;
	}

private:
	static BindableList staticBinds;
	static HeapResource heapResource;
};

template<class T>
Drawable::BindableList DrawableBase<T>::staticBinds = {};

template<class T>
HeapResource DrawableBase<T>::heapResource = {};
