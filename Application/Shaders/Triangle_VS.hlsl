#include "Triangle.hlsli"

VertexOut VS(VertexIn vIn){
	VertexOut vOut;
	vOut.posH=float4(vIn.pos,1.0f);
	vOut.color=vIn.color;
	return vOut;
}