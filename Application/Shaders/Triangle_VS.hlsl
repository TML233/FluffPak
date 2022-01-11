#include "Triangle.hlsli"

VertexOut VS(VertexIn vIn){
	VertexOut vOut;
	vOut.pos = mul(float4(vIn.pos, 1.0f), gWorld);
	vOut.pos = mul(vOut.pos, gView);
	vOut.pos = mul(vOut.pos, gProjection);
	vOut.color=vIn.color;
	return vOut;
}