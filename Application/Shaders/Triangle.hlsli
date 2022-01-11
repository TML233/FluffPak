cbuffer TransformMatrices : register(b0) {
	matrix gWorld;
	matrix gView;
	matrix gProjection;
}

struct VertexIn{
	float3 pos:POSITION;
	float4 color:COLOR;
};

struct VertexOut{
	float4 pos:SV_POSITION;
	float4 color:COLOR;
};