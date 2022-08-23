#include "RootSignature.h"
#include "GraphicContext.h"

RootSignature::RootSignature(Graphics& gfx, std::unique_ptr<SignatureNode> root)
	: mRoot(std::move(root))
{
	// Create a root signature.
	D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
	featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

	auto device = GraphicContext::GetDevice(gfx);
	if (FAILED(device->CheckFeatureSupport(
		D3D12_FEATURE_ROOT_SIGNATURE, 
		&featureData, 
		sizeof(featureData))))
	{
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
	}

	std::vector<CD3DX12_ROOT_PARAMETER1> rootParameters;
	std::vector<std::vector<CD3DX12_DESCRIPTOR_RANGE1>> parameterTableRanges;

	rootParameters.resize(mRoot->Count());

	UINT index = 0;
	for (auto& param : mRoot->GetSubNodes())
	{
		if (param->GetType() == ParameterType::Constant)
		{
			rootParameters[index].InitAsConstants(param->GetNumDescriptor(), param->GetBaseRegister(), 0, param->GetVisibility());
		}
		else if (param->GetType() == ParameterType::TABLE)
		{
			std::vector<CD3DX12_DESCRIPTOR_RANGE1> descriptorRages;
			descriptorRages.resize(param->Count());

			UINT rangeIndex = 0;
			for (auto& range : param->GetSubNodes())
			{
				D3D12_DESCRIPTOR_RANGE_TYPE rangeType;
				switch (range->GetType())
				{
				case ParameterType::CBV:
					rangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
					break;
				case ParameterType::SRV:
					rangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
					break;
				case ParameterType::UAV:
					rangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
					break;
				case ParameterType::SAMPLER:
					rangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
					break;
				default:
					throw std::exception();
				}
				descriptorRages[rangeIndex].Init(rangeType, range->GetNumDescriptor(), range->GetBaseRegister());
				rangeIndex++;
			}
			parameterTableRanges.push_back(descriptorRages);
			rootParameters[index].InitAsDescriptorTable((UINT)descriptorRages.size(), parameterTableRanges.back().data(), param->GetVisibility());
		}
		index++;
	}

	CD3DX12_STATIC_SAMPLER_DESC linearRepeatSampler(0, D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);
	// Allow input layout and deny unnecessary access to certain pipeline stages.
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDescription;
	rootSignatureDescription.Init_1_1(
		(UINT)rootParameters.size(),
		rootParameters.data(), 
		1, 
		&linearRepeatSampler, 
		rootSignatureFlags);

	// Serialize the root signature.
	ComPtr<ID3DBlob> rootSignatureBlob;
	ComPtr<ID3DBlob> errorBlob;
	ThrowIfFailed(D3DX12SerializeVersionedRootSignature(
		&rootSignatureDescription,
		featureData.HighestVersion,
		&rootSignatureBlob,
		&errorBlob));

	// Create the root signature.
	ThrowIfFailed(device->CreateRootSignature(
		0,
		rootSignatureBlob->GetBufferPointer(),
		rootSignatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&m_RootSignature)));
}

void RootSignature::Bind( Graphics& gfx )
{
	GraphicContext::GetCommandList( gfx )->SetGraphicsRootSignature(m_RootSignature.Get());;
}
