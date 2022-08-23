struct ModelViewProjection
{
	matrix MVP;
};

struct VSOut
{
	float2 tex : PixelTexCoord;
	float4 pos : SV_Position;
};

ConstantBuffer<ModelViewProjection> modelViewProjectionCB: register(b0);

VSOut main( float3 pos : Position,float2 tex : TexCoord )
{
	VSOut vso;
	vso.pos = mul(modelViewProjectionCB.MVP, float4(pos, 1.0f)); 
	vso.tex = tex;
	return vso;
}