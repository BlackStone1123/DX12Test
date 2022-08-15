#include "Box.h"
#include "BindableBase.h"
#include "Vertex.h"

using namespace DirectX;

Box::Box( Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist )
	: x( rdist( rng ) )
	, droll( ddist( rng ) )
	, dpitch( ddist( rng ) )
	, dyaw( ddist( rng ) )
	, dphi( odist( rng ) )
	, dtheta( odist( rng ) )
	, dchi( odist( rng ) )
	, chi( adist( rng ) )
	, theta( adist( rng ) )
	, phi( adist( rng ) )
{
	if( !IsStaticInitialized() )
	{
		const std::vector<Vertex> vertices =
		{
		    { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) }, // 0
			{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) }, // 1
			{ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f) }, // 2
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) }, // 3
			{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) }, // 4
			{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f, 1.0f, 1.0f) }, // 5
			{ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f) }, // 6
			{ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(1.0f, 0.0f, 1.0f) }
		};

		AddStaticBind( std::make_unique<VertexBuffer<Vertex>>( gfx,vertices ) );

		const std::vector<unsigned short> indices =
		{
		    0, 1, 2, 0, 2, 3,
			4, 6, 5, 4, 7, 6,
			4, 5, 1, 4, 1, 0,
			3, 2, 6, 3, 6, 7,
			1, 5, 6, 1, 6, 2,
			4, 0, 3, 4, 3, 7
		};
		AddStaticIndexBuffer( std::make_unique<IndexBuffer>( gfx,indices ) );

		//AddStaticBind( std::make_unique<PixelConstantBuffer<ConstantBuffer2>>( gfx,cb2 ) );

		AddStaticBind( std::make_unique<Topology>( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

		auto pvs = std::make_unique<Shader>(gfx, L"VertexShader.cso");
		auto pps = std::make_unique<Shader>(gfx, L"PixelShader.cso");
		const std::vector<D3D12_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
			{ "COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
		};
		
		auto sig = std::make_unique<RootSignature>(gfx);
		auto pso = std::make_unique<PiplineStateObject>(gfx, ied, sig->GetSignature(), pvs->GetBytecode(), pps->GetBytecode());
		AddStaticBind(std::move(sig));
		AddStaticBind(std::move(pso));

	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind( std::make_unique<TransformCbuf>( gfx,*this ) );
}

void Box::Update(Graphics& gfx, float dt )
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Box::GetTransformXM() const
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(x, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}
