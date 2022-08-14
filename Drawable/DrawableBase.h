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
		if (auto buffer = dynamic_cast<Buffer*>(bind.get()))
		{
			AddBuffer(buffer);
		}
		staticBinds.push_back( std::move( bind ) );
	}
	void AddStaticIndexBuffer( std::unique_ptr<IndexBuffer> ibuf )
	{
		pIndexBuffer = ibuf.get();
		AddBuffer(ibuf.get());
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

private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const override
	{
		return staticBinds;
	}

private:
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds = {};