#pragma once
#include "Bindable.h"

enum class ParameterType: std::uint8_t
{
	Constant = 0,
	TABLE,
	SRV,
	CBV,
	UAV,
	SAMPLER
};

class SignatureNode
{
public:
	using SubNodeList = std::vector<std::unique_ptr<SignatureNode>>;

	SignatureNode() {}
	~SignatureNode() = default;

	UINT GetBaseRegister() const { return mBase; }
	SignatureNode* SetBaseRegister(UINT base) { mBase = base; return this; }

	UINT GetNumDescriptor() const { return mNum; }
	SignatureNode* SetNumDescriptor(UINT num) { mNum = num; return this; }

	ParameterType GetType() const { return mType; }
	SignatureNode* SetType(ParameterType type) { mType = type; return this; }

	UINT Count() const { return (UINT)mChildren.size(); }
	SignatureNode* AddSubNode() 
	{
		auto newNode = std::make_unique<SignatureNode>();
		mChildren.emplace_back(std::move(newNode));
		return mChildren.back().get();
	}
	const SubNodeList& GetSubNodes() const { return mChildren; }

private:
	ParameterType mType;
	UINT mNum{ 0 };
	UINT mBase{ 0 };
	SubNodeList mChildren;
};

class RootSignature : public Bindable
{
public:
	RootSignature( Graphics& gfx, std::unique_ptr<SignatureNode> root);
	void Bind( Graphics& gfx ) override;
	ComPtr<ID3D12RootSignature> GetSignature() 
	{
		return m_RootSignature;
	}

private:
	// Root signature
	ComPtr<ID3D12RootSignature> m_RootSignature;
	std::unique_ptr<SignatureNode> mRoot;
};