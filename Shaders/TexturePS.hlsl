cbuffer SceneConstantBuffer
{
    int index;
    float padding[63];
};

Texture2D tex : register(t0);
Texture2D tex1 : register(t1);
SamplerState splr : register(s0);

[earlydepthstencil]
float4 main(float2 texCoord : PixelTexCoord) : SV_Target
{
	return index % 2 == 0 ? tex1.Sample(splr,texCoord) : tex.Sample(splr, texCoord);
}