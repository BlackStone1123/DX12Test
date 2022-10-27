#include "Topology.h"
#include "GraphicContext.h"
#include "BindableCodex.h"

Topology::Topology( Graphics& gfx,D3D12_PRIMITIVE_TOPOLOGY type )
	: type( type )
{

}

void Topology::Bind( Graphics& gfx )
{
	GraphicContext::GetCommandList( gfx )->IASetPrimitiveTopology( type );
}

std::shared_ptr<Topology> Topology::Resolve(Graphics& gfx, D3D12_PRIMITIVE_TOPOLOGY type)
{
	return Codex::Resolve<Topology>(gfx, type);
}
std::string Topology::GenerateUID(D3D12_PRIMITIVE_TOPOLOGY type)
{
	using namespace std::string_literals;
	return typeid(Topology).name() + "#"s + std::to_string(type);
}