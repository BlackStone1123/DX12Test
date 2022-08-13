#include "Topology.h"
#include "GraphicContext.h"

Topology::Topology( Graphics& gfx,D3D12_PRIMITIVE_TOPOLOGY type )
	: type( type )
{

}

void Topology::Bind( Graphics& gfx )
{
	GraphicContext::GetCommandList( gfx )->IASetPrimitiveTopology( type );
}
