#include "SolidSphere.h"
#include "BindableBase.h"
#include "Geometry/Sphere.h"

namespace dx = DirectX;
//SolidSphere::SolidSphere( Graphics& gfx,float radius )
//{
//	if( !IsStaticInitialized() )
//	{
//		struct Vertex
//		{
//			dx::XMFLOAT3 pos;
//		};
//		auto model = Sphere::Make<Vertex>();
//		//model.Transform( dx::XMMatrixScaling( radius,radius,radius ) );
//		AddStaticBind( std::make_unique<VertexBuffer<Vertex>>( gfx,model.vertices ) );
//		AddIndexBuffer( std::make_unique<IndexBuffer>( gfx,model.indices ) );
//		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
//
//		auto pvs = std::make_unique<Shader>( gfx,L"SolidVS.cso" );
//		auto pps = std::make_unique<Shader>(gfx, L"SolidPS.cso");
//
//		const std::vector<D3D12_INPUT_ELEMENT_DESC> ied =
//		{
//			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
//		};
//		auto sig = std::make_unique<RootSignature>(gfx);
//		auto pso = std::make_unique<PiplineStateObject>(gfx, ied, sig->GetSignature(), pvs->GetBytecode(), pps->GetBytecode());
//		AddStaticBind(std::move(sig));
//		AddStaticBind(std::move(pso));
//	}
//	else
//	{
//		SetIndexFromStatic();
//	}
//
//	AddBind( std::make_unique<TransformCbuf>( gfx,*this ) );
//}
//
//void SolidSphere::Update( float dt ) noexcept {}
//
//void SolidSphere::SetPos( DirectX::XMFLOAT3 pos ) noexcept
//{
//	this->pos = pos;
//}
//
//DirectX::XMMATRIX SolidSphere::GetTransformXM() const noexcept
//{
//	return DirectX::XMMatrixTranslation( pos.x,pos.y,pos.z );
//}
