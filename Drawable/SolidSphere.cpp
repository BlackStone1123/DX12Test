#include "SolidSphere.h"
#include "BindableBase.h"
#include "Geometry/Sphere.h"

namespace dx = DirectX;

SolidSphere::SolidSphere( Graphics& gfx,float radius, const DirectX::XMFLOAT3& color)
	: mRadius(radius)
{
	struct Vertex
	{
		dx::XMFLOAT3 pos;
	};
	auto model = Sphere::Make<Vertex>();

	AddBind(VertexBuffer<Vertex>::Resolve( gfx,"SolidSphereVertex", model.vertices));
	AddBind(IndexBuffer::Resolve( gfx, "SolidSphereIndex",model.indices ) );
	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind( std::make_unique<TransformCbuf>( gfx,*this, 0) );
}

void SolidSphere::Update( float dt ) {}

void SolidSphere::SetPos( DirectX::XMFLOAT4 pos )
{
	this->pos = pos;
}

DirectX::XMMATRIX SolidSphere::GetTransformXM() const
{
	auto scalingMatrix = dx::XMMatrixScaling(mRadius, mRadius, mRadius);
	auto translationMatrix = dx::XMMatrixTranslation(pos.x, pos.y, pos.z);
	return scalingMatrix * translationMatrix;
}
