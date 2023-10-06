#include "x3dshaders.h"

const char *X3D_VS_XYZ_SHADER = R"(
float4x4 world;
float4x4 view;
float4x4 proj;

struct VS_INPUT
{
    float3 vPosition : POSITION;
};

struct VS_OUTPUT
{
    float4 vPosition : POSITION;
};

VS_OUTPUT vs_main(VS_INPUT v)
{
    VS_OUTPUT o;

    float4 wp = mul(float4(v.vPosition, 1), world);
    o.vPosition = mul(mul(wp, view), proj);

    return o;
}
)";

const char *X3D_PS_XYZ_SHADER = R"(
struct PS_INPUT
{
    float4 vPosition : POSITION;
};

float4 ps_main(PS_INPUT v) : COLOR0
{
    float4 c = float4(1, 0, 1, 1);
	return c;
}
)";

const char *X3D_VS_XYZ_NORM_SHADER = R"(
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
    float4 vPosition : POSITION;
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
)";

const char *X3D_PS_XYZ_NORM_SHADER = R"(
struct PS_INPUT
{
    float4 vPosition : POSITION;
    float4 vNormal : NORMAL;
};

float4 ps_main(PS_INPUT v) : COLOR0
{
    float4 c = float4(1, 0, 1, 1);
	return c;
}
)";