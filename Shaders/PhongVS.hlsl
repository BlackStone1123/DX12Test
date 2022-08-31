struct PositionMatrix
{
	matrix model;
	matrix modelView;
	matrix modelViewProj;
	matrix InverseTransposeModelViewMatrix;
};

ConstantBuffer<PositionMatrix> positionMatrix : register(b0);

struct VSOut
{
	float4 worldPos : Position;
	float3 normal : Normal;
	float2 texCoord: TEXCOORD;
	float4 pos : SV_Position;
};

VSOut main( float3 pos : Position,float3 n : Normal, float2 texCoord: TEXCOORD )
{
	VSOut vso;
    vso.worldPos = mul(positionMatrix.modelView, float4(pos, 1.0f));
	vso.pos = mul(positionMatrix.modelViewProj, float4(pos, 1.0f));
	vso.normal = mul((float3x3)positionMatrix.InverseTransposeModelViewMatrix, n);
	vso.texCoord = texCoord;
	return vso;
}