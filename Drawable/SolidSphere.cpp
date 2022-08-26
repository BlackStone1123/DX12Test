#include "SolidSphere.h"
#include "BindableBase.h"
#include "Geometry/Sphere.h"

namespace dx = DirectX;

SolidSphere::SolidSphere( Graphics& gfx,float radius, const DirectX::XMFLOAT3& color)
	: mRadius(radius)
{
	if( !IsStaticInitialized() )
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
		};
		auto model = Sphere::Make<Vertex>();

		AddStaticBind( std::make_unique<VertexBuffer<Vertex>>( gfx,model.vertices ) );
		AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx,model.indices ) );
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		auto pvs = std::make_unique<Shader>( gfx,L"SolidVS.cso" );
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

		AddStaticBind(std::move(sig));
		AddStaticBind(std::move(pso));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind( std::make_unique<TransformCbuf>( gfx,*this, 0) );
	AddBind(std::make_unique<ColorCbuf>(gfx, color, 1));
}

void SolidSphere::Update( float dt ) {}

void SolidSphere::SetPos( DirectX::XMFLOAT3 pos )
{
	this->pos = pos;
}

DirectX::XMMATRIX SolidSphere::GetTransformXM() const
{
	auto scalingMatrix = dx::XMMatrixScaling(mRadius, mRadius, mRadius);
	auto translationMatrix = dx::XMMatrixTranslation(pos.x, pos.y, pos.z);
	return scalingMatrix * translationMatrix;
}
