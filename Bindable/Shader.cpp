#include "Shader.h"

Shader::Shader( Graphics& gfx,const std::wstring& path )
{
	ThrowIfFailed( D3DReadFileToBlob( path.c_str(),&pBytecodeBlob ) );

	ComPtr<ID3DBlob> asmBlob;
	ThrowIfFailed(D3DDisassemble(
		pBytecodeBlob->GetBufferPointer(),
		pBytecodeBlob->GetBufferSize(),
		D3D_DISASM_ENABLE_INSTRUCTION_NUMBERING,
		"",
		&asmBlob
	));
	fwrite(asmBlob->GetBufferPointer(), 1, asmBlob->GetBufferSize(), stdout);
}

void Shader::Bind( Graphics& gfx )
{

}

ComPtr<ID3DBlob> Shader::GetBytecode() const
{
	return pBytecodeBlob;
}
