struct PositionMatrix
{
	matrix model;
	matrix modelView;
	matrix modelViewProj;
	matrix InverseTransposeModelViewMatrix;
};
ConstantBuffer<PositionMatrix> modelViewProjectionCB: register(b0);

float4 main(float3 pos : Position) : SV_Position
{
	return mul(modelViewProjectionCB.modelViewProj,float4(pos,1.0f));
}