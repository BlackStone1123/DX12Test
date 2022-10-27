#include "Pass.h"
#include "Shader.h"
#include "RootSignature.h"
#include "Graphics.h"
#include "PiplineStateObject.h"
#include "TransformCbuf.h"
#include "Material.h"
#include "PointLight.h"

void Pass::AttachDrawable(Drawable* drawable)
{
	mDrawables.push_back(drawable);
}

void Pass::Execute(Graphics& gfx)
{
	for (auto& bind : mBindables)
	{
		bind->Bind(gfx);
	}
	for (auto& drawable : mDrawables)
	{
		drawable->Draw(gfx);
	}

	mDrawables.clear();
}
/////////////////////////////////////////////////////////////////////////////////
SolidColorPass::SolidColorPass(Graphics& gfx, const DirectX::XMFLOAT3& color)
{
	auto pvs = std::make_unique<Shader>(gfx, L"SolidVS.cso");
	auto pps = std::make_unique<Shader>(gfx, L"SolidPS.cso");

	const std::vector<D3D12_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	};

	auto root = std::make_unique<SignatureNode>();
	root->AddSubNode()
		->SetType(ParameterType::CBV)
		->SetBaseRegister(0)
		->SetVisibility(D3D12_SHADER_VISIBILITY_VERTEX);

	root->AddSubNode()
		->SetType(ParameterType::Constant)
		->SetNumDescriptor(sizeof(DirectX::XMFLOAT4) / 4)
		->SetBaseRegister(0)
		->SetVisibility(D3D12_SHADER_VISIBILITY_PIXEL);

	auto sig = std::make_unique<RootSignature>(gfx, std::move(root));
	auto pso = std::make_unique<PiplineStateObject>(
		gfx,
		ied,
		sig->GetSignature(),
		pvs->GetBytecode(),
		pps->GetBytecode()
		);

	mBindables.push_back(std::move(sig));
	mBindables.push_back(std::move(pso));
	mBindables.push_back(std::make_unique<ColorCbuf>(gfx, color, 1));
}

void SolidColorPass::AttachDrawable(Drawable* drawable)
{
	drawable->getTransformCBuf()->SetBindSlot(0);
	Pass::AttachDrawable(drawable);
}

BorderPass::BorderPass(Graphics& gfx, const DirectX::XMFLOAT3& color)
{
	auto pvs = std::make_unique<Shader>(gfx, L"SolidVS.cso");
	auto pps = std::make_unique<Shader>(gfx, L"SolidPS.cso");

	const std::vector<D3D12_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
	};

	auto root = std::make_unique<SignatureNode>();
	root->AddSubNode()
		->SetType(ParameterType::CBV)
		->SetBaseRegister(0)
		->SetVisibility(D3D12_SHADER_VISIBILITY_VERTEX);

	root->AddSubNode()
		->SetType(ParameterType::Constant)
		->SetNumDescriptor(sizeof(DirectX::XMFLOAT4) / 4)
		->SetBaseRegister(0)
		->SetVisibility(D3D12_SHADER_VISIBILITY_PIXEL);

	auto sig = std::make_unique<RootSignature>(gfx, std::move(root));
	auto pso = std::make_unique<PiplineStateObject>(
		gfx,
		ied,
		sig->GetSignature(),
		pvs->GetBytecode(),
		pps->GetBytecode(),
		StencilMode::Mask
		);

	mBindables.push_back(std::move(sig));
	mBindables.push_back(std::move(pso));
	mBindables.push_back(std::make_unique<ColorCbuf>(gfx, color, 1));
}

void BorderPass::AttachDrawable(Drawable* drawable)
{
	drawable->getTransformCBuf()->SetBindSlot(0);
	Pass::AttachDrawable(drawable);
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
struct CData
{
	UINT index{ 0 };
	float padding[63];
};

class ChangeIndex : public ConstantBuffer<CData>
	, public ImguiItem
{
public:
	ChangeIndex(Graphics& gfx, const CData& data, const HeapAllocation& location)
		: Super(gfx, data, location)
	{
		gfx.AddImguiItem(this);
	}

	ChangeIndex(Graphics& gfx, const CData& data)
		: Super(gfx, data)
	{
		gfx.AddImguiItem(this);
	}

	virtual void Display() override
	{
		if (ImGui::CollapsingHeader("Texture Index"))
		{
			static bool check = true;
			ImGui::Checkbox("checkbox", &check);
			GetRawData().index = check ? 0 : 1;
		}
	}
};

PhongShaderPass::PhongShaderPass(Graphics& gfx)
{
	auto pvs = std::make_unique<Shader>(gfx, L"PhongVS.cso");
	auto pps = std::make_unique<Shader>(gfx, L"PhongPS.cso");
	const std::vector<D3D12_INPUT_ELEMENT_DESC> ied =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
	};

	auto root = std::make_unique<SignatureNode>();
	root->AddSubNode()
		->SetType(ParameterType::CBV)
		->SetBaseRegister(0)
		->SetVisibility(D3D12_SHADER_VISIBILITY_VERTEX);

	root->AddSubNode()
		->SetType(ParameterType::CBV)
		->SetBaseRegister(0)
		->SetVisibility(D3D12_SHADER_VISIBILITY_PIXEL);

	root->AddSubNode()
		->SetType(ParameterType::CBV)
		->SetBaseRegister(1)
		->SetVisibility(D3D12_SHADER_VISIBILITY_PIXEL);

	root->AddSubNode()
		->SetType(ParameterType::TABLE)
		->SetVisibility(D3D12_SHADER_VISIBILITY_PIXEL)
		->AddSubNode()
		->SetType(ParameterType::SRV)
		->SetNumDescriptor(3)
		->SetBaseRegister(0);

	auto sig = std::make_unique<RootSignature>(gfx, std::move(root));
	auto pso = std::make_unique<PiplineStateObject>(
		gfx, ied, 
		sig->GetSignature(),
		pvs->GetBytecode(),
		pps->GetBytecode(),
		StencilMode::Write);

	mBindables.push_back(std::move(pso));
	mBindables.push_back(std::move(sig));

	auto changeIndexBuf = std::make_unique<ChangeIndex>(gfx, CData());
	changeIndexBuf->SetBindSlot(1);
	mBindables.push_back(std::move(changeIndexBuf));
}
void PhongShaderPass::SetPointLight(PointLight* pt)
{
	mPt = pt;
	mPt->SetBindSlot(2);
}

void PhongShaderPass::AttachDrawable(Drawable* drawable)
{
	drawable->getTransformCBuf()->SetBindSlot(0);
	drawable->GetMaterial()->source().SetBindSlot(3);
	Pass::AttachDrawable(drawable);
}

void PhongShaderPass::Execute(Graphics& gfx)
{
	for (auto& bind : mBindables)
	{
		bind->Bind(gfx);
	}

	if (mPt)
	{
		mPt->Bind(gfx);
	}

	for (auto& drawable : mDrawables)
	{
		drawable->Draw(gfx);
	}

	mDrawables.clear();
}