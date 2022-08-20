cbuffer SceneConstantBuffer : register(b1)
{
    int index;
    float padding[63];
};

Texture2D tex : register(t0);
Texture2D tex1 : register(t1);
SamplerState splr : register(s0);

float4 main(float2 tc : TexCoord) : SV_Target
{
	return index % 2 == 0 ? tex1.Sample(splr,tc) : tex.Sample(splr, tc);
}