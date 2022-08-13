#include "Shader.h"

Shader::Shader( Graphics& gfx,const std::wstring& path )
{
	ThrowIfFailed( D3DReadFileToBlob( path.c_str(),&pBytecodeBlob ) );
}

void Shader::Bind( Graphics& gfx )
{

}

ComPtr<ID3DBlob> Shader::GetBytecode() const
{
	return pBytecodeBlob;
}
