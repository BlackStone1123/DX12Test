#pragma once
#include "Bindable.h"

class IndexBuffer : public Bindable
				  , public Resource
{
public:
	IndexBuffer( Graphics& gfx,const std::string& tag, const std::vector<unsigned short>& indices );
	UINT GetCount() const;

public:
	virtual void Bind(Graphics& gfx) override;
	virtual void Upload(Graphics& gfx) override;

	static std::shared_ptr<IndexBuffer> Resolve(
		Graphics& gfx, 
		const std::string& tag, 
		const std::vector<unsigned short>& indices);

	template<typename...Ignore>
	static std::string GenerateUID(const std::string& tag, Ignore&&...ignore)
	{
		using namespace std::string_literals;
		return typeid(IndexBuffer).name() + "#"s + tag;
	}
protected:
	UINT count;
	std::string mTag;

	std::vector<unsigned short> m_Indices;
	ComPtr<ID3D12Resource> m_IndexBuffer;
	ComPtr<ID3D12Resource> m_IntermediateIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
};