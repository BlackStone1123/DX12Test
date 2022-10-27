#include "Box.h"
#include "BindableBase.h"
#include "Vertex.h"
#include "Material.h"
#include "PointLight.h"
#include "Geometry/Cube.h"

using namespace DirectX;

struct CData
{
	UINT index{ 0 };
	float padding[63];
};

class ChangeIndex : public ConstantBuffer<CData>
				  , public ImguiItem
{
public:
	ChangeIndex(Graphics& gfx,const CData& data, const HeapAllocation& location)
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
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 tex;
		DirectX::XMFLOAT3 normal;
	};

	auto model = Cube::MakeIndependent<Vertex>();
	model.SetNormalsIndependentFlat();

	AddBind(VertexBuffer<Vertex>::Resolve(gfx,"TestBoxVertex", model.vertices));
	AddBind(IndexBuffer::Resolve(gfx,"TestBoxIndex", model.indices));
	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

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
	auto pso = std::make_unique<PiplineStateObject>(gfx, ied, sig->GetSignature(), pvs->GetBytecode(), pps->GetBytecode());
	AddBind(std::move(pso));
	AddBind(std::move(sig));

	auto changeIndexBuf = std::make_unique<ChangeIndex>(gfx, CData());
	changeIndexBuf->SetBindSlot(1);
	AddBind(std::move(changeIndexBuf));
	AddBind(std::make_unique<PointLightReference>(gfx.GetPointLight(), 2));
	AddBind( std::make_unique<TransformCbuf>( gfx,*this, 0) );
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
	auto xf = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(x, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);

	if (outlining)
	{
		xf = DirectX::XMMatrixScaling(1.03f, 1.03f, 1.03f) * xf;
	}
	return xf;
}