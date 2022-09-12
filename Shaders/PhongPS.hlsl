cbuffer SceneConstantBuffer : register(b0)
{
	int index;
	float padding[63];
};

struct Light
{
	float4 lightPos;
	float4 materialColor;
	float4 ambient;
	float4 diffuseColor;
	float diffuseIntensity;
	float attConst;
	float attLin;
	float attQuad;
};

ConstantBuffer<Light> lightProperty : register(b1);

Texture2D tex : register(t0);
Texture2D tex1 : register(t1);
Texture2D texSpec : register(t2);

SamplerState splr : register(s0);

static float4 specularIntensity = { 1.0f,1.0f,1.0f,1.0f };
static float specularPower = 128.0f;

float4 main( float3 worldPos : Position,float3 n : Normal, float2 texCoord : TEXCOORD ) : SV_Target
{
	// fragment to light vector data
	const float3 vToL = lightProperty.lightPos.xyz - worldPos.xyz;
	const float distToL = length( vToL );
	const float3 dirToL = vToL / distToL;
	const float3 dirRef = normalize(reflect(-dirToL, n));
	const float3 dirView = normalize(-worldPos);

	// attenuation
	const float att = 1.0f / (lightProperty.attConst + lightProperty.attLin * distToL + lightProperty.attQuad * (distToL * distToL));

	// diffuse intensity
	const float4 diffuse = lightProperty.diffuseColor * lightProperty.diffuseIntensity * att * max( 0.0f,dot( dirToL,n ) );

	// specular
	float RdotV = max(0.0f, dot(dirRef, dirView));
    const float4 specular = att * specularIntensity * pow(RdotV, specularPower);
    const float4 materialColor = index % 2 == 0 ? tex.Sample(splr, texCoord) : tex1.Sample(splr, texCoord);
	const float4 specColor = texSpec.Sample(splr, texCoord);

	// final color
    return saturate(diffuse + lightProperty.ambient) * materialColor + saturate(specular) * specColor;
}