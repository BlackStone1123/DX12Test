#include "Box.h"
#include "BindableBase.h"
#include "Vertex.h"
#include "Material.h"

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
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 color;
			DirectX::XMFLOAT2 tex;
		};

		const std::vector<Vertex> vertices =
		{
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), {0.0f,1.0f} }, // 0
			{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), {0.0f, 0.0f} }, // 1
			{ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f), {1.0f,0.0f} }, // 2
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), {1.0f,1.0f} }, // 3
			{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), {1.0f,1.0f} }, // 4
			{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f, 1.0f, 1.0f), {1.0f,0.0f} }, // 5
			{ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), {0.0f, 0.0f} }, // 6
			{ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(1.0f, 0.0f, 1.0f), {0.0f,1.0f} }
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
		AddStaticBind( std::make_unique<Topology>( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

		auto pvs = std::make_unique<Shader>(gfx, L"TextureVS.cso");
		auto pps = std::make_unique<Shader>(gfx, L"TexturePS.cso");
		const std::vector<D3D12_INPUT_ELEMENT_DESC> ied =
		{
			{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
			{ "COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 },
			{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 }
		};
		
		auto root = std::make_unique<SignatureNode>();
		root->AddSubNode()->SetType(ParameterType::Constant)->SetNumDescriptor(sizeof(DirectX::XMMATRIX) / 4)->SetBaseRegister(0);

		auto tableNode = root->AddSubNode()->SetType(ParameterType::TABLE);
		tableNode->AddSubNode()->SetType(ParameterType::SRV)->SetNumDescriptor(2)->SetBaseRegister(0);
		tableNode->AddSubNode()->SetType(ParameterType::CBV)->SetNumDescriptor(1)->SetBaseRegister(1);

		auto sig = std::make_unique<RootSignature>(gfx, std::move(root));
		auto pso = std::make_unique<PiplineStateObject>(gfx, ied, sig->GetSignature(), pvs->GetBytecode(), pps->GetBytecode());
		AddStaticBind(std::move(sig));
		AddStaticBind(std::move(pso));

		auto mat = std::make_unique<Material>(gfx, 2);
		mat->AddTexture(std::make_unique<Surface>( L"../../../Assets/Texture/Mona_Lisa.jpg", true));
		mat->AddTexture(std::make_unique<Surface>( L"../../../Assets/Texture/kappa50.png", true));
		AddStaticBind(std::move(mat));
		AddStaticBind(std::move(std::make_unique<ChangeIndex>(gfx)));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind( std::make_unique<TransformCbuf>( gfx,*this ) );
}

Box::~Box()
{

}

void Box::Update(float dt )
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
