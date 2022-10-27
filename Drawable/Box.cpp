#include "Box.h"
#include "BindableBase.h"
#include "Vertex.h"
#include "Material.h"
#include "PointLight.h"
#include "Geometry/Cube.h"

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

	return xf;
}

Border::Border(Graphics& gfx)
{
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 tex;
		DirectX::XMFLOAT3 normal;
	};

	auto model = Cube::MakeIndependent<Vertex>();
	model.SetNormalsIndependentFlat();

	AddBind(VertexBuffer<Vertex>::Resolve(gfx, "TestBoxVertex", model.vertices));
	AddBind(IndexBuffer::Resolve(gfx, "TestBoxIndex", model.indices));
	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_unique<TransformCbuf>(gfx, *this, 0));
}

Border::~Border()
{

}

DirectX::XMMATRIX Border::GetTransformXM() const
{
	return DirectX::XMMatrixScaling(1.1f, 1.1f, 1.1f) * mMatrix;
}

void Border::setTransformXM(const DirectX::XMMATRIX& mat)
{
	mMatrix = mat;
}