#include "x3dshaders.h"

const char *X3D_XYZ_SHADER = R"(
struct VS_INPUT
{
    float3 vPosition : POSITION;
};

struct VS_OUTPUT
{
    float4 vPosition : VPOS;
};

VS_OUTPUT vs_main(VS_INPUT v)
{
    VS_OUTPUT o;

    float4 wp = mul(float4(v.vPosition, 1), world);
    o.vPosition = mul(mul(wp, view), proj);

    return o;
}

float4 ps_main(VS_OUTPUT v) : COLOR0
{
    float4 c = v.vPosition;
	return c;
}
)";

const char *X3D_XYZ_NORM_SHADER = R"(
float4x4 world;
float4x4 view;
float4x4 proj;

struct VS_INPUT
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
};

struct VS_OUTPUT
{
    float4 vPosition : VPOS;
    float4 vNormal : NORMAL;
};

VS_OUTPUT vs_main(VS_INPUT v)
{
    VS_OUTPUT o;

    float4 wp = mul(float4(v.vPosition, 1), world);
    o.vPosition = mul(mul(wp, view), proj);
    o.vNormal = float4(v.vNormal, 0);

    return o;
}

float4 ps_main(VS_OUTPUT v) : COLOR0
{
    float4 c = v.vPosition;
	return c;
}
)";

const char *X3D_XYZ_NORM_TX1_SHADER = R"(
float4x4 world;
float4x4 view;
float4x4 proj;

struct VS_INPUT
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexCoord0  : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 vPosition : VPOS;
    float4 vNormal : NORMAL;
    float2 vTexCoord0  : TEXCOORD0;
};

VS_OUTPUT vs_main(VS_INPUT v)
{
    VS_OUTPUT o;

    float4 wp = mul(float4(v.vPosition, 1), world);
    o.vPosition = mul(mul(wp, view), proj);
    o.vNormal = float4(v.vNormal, 0);
    o.vTexCoord0 = v.vTexCoord0;

    return o;
}

struct LightSource
{
    float3 lPosition;
    float3 lDirection;
};

uniform sampler2D Tex0 : register(s0); 
uniform float3 AmbientColor;
uniform float3 DiffuseColor;
uniform LightSource Light0;

float4 ps_main(VS_OUTPUT v) : COLOR0
{
    float4 c;
    c = tex2D(Tex0, v.vTexCoord0);
    if(c.a < 0.01) {
        discard;
    }
    // Apply ambient material color
    c *= float4(AmbientColor, 1.0);
	return c;
}
)";