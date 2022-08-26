cbuffer SceneConstantBuffer : register(b0)
{
	int index;
	float padding[63];
};

struct Light
{
	float3 lightPos;
	float3 materialColor;
	float3 ambient;
	float3 diffuseColor;
	float diffuseIntensity;
	float attConst;
	float attLin;
	float attQuad;
};

ConstantBuffer<Light> lightProperty : register(b1);

Texture2D tex : register(t0);
Texture2D tex1 : register(t1);
SamplerState splr : register(s0);

float4 main( float3 worldPos : Position,float3 n : Normal, float2 texCoord : TEXCOORD ) : SV_Target
{
	// fragment to light vector data
	const float3 vToL = lightProperty.lightPos - worldPos;
	const float distToL = length( vToL );
	const float3 dirToL = vToL / distToL;
	// diffuse attenuation
	const float att = 1.0f / (lightProperty.attConst + lightProperty.attLin * distToL + lightProperty.attQuad * (distToL * distToL));
	// diffuse intensity
	const float3 diffuse = lightProperty.diffuseColor * lightProperty.diffuseIntensity * att * max( 0.0f,dot( dirToL,n ) );
	const float4 materialColor = tex1.Sample(splr, texCoord);

	// final color
	return float4(saturate( (diffuse + lightProperty.ambient) * materialColor ),1.0f);
}