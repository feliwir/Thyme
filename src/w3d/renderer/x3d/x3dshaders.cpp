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

// Use: https://bgolus.medium.com/anti-aliased-alpha-test-the-esoteric-alpha-to-coverage-8b177335ae4f
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
    o.vNormal = mul(float4(v.vNormal, 0), world);
    o.vTexCoord0 = v.vTexCoord0;

    return o;
}

struct LightSource
{
    int lType;
    float3 lPosition;
    float3 lDirection;
    float3 lDiffuseColor;
    float3 lSpecularColor;
};

// Material
uniform sampler2D Tex0 : register(s0); 
uniform float3 MaterialAmbientColor;
uniform float3 MaterialDiffuseColor;
uniform float3 MaterialEmissiveColor;
uniform float3 MaterialSpecularColor;

// Light
uniform LightSource Lights[4];
// This contains the sum of ambient color for all lights + the scene ambient color
uniform float3 LightAmbientColor;

float4 ps_main(VS_OUTPUT v) : COLOR0
{
    float4 c;
    c = tex2D(Tex0, v.vTexCoord0);
    if(c.a < 0.4) {
        discard;
    }
    // Apply ambient material color
    c *= float4(MaterialAmbientColor, 1.0);

    // Apply ambient light color
    float3 lightColor = LightAmbientColor;

    for(int lIndex = 0; lIndex<4; lIndex++)
    {
        LightSource light = Lights[lIndex];
        // No light
        if(light.lType == 0) {
            continue;
        }
        float3 lightDir = 0.0;
        // Directional light
        if(light.lType == 1) {
            lightDir = -light.lDirection;
        }
        // Point light
        if(light.lType == 2) {
            lightDir = light.lPosition - v.vPosition.xyz;
        }
        float lightIntensity = max(dot(normalize(lightDir), normalize(v.vNormal.xyz)), 0.0);
        lightColor += light.lDiffuseColor * lightIntensity;
    }
    lightColor = clamp(lightColor, 0.0, 1.0);
	return float4(lightColor, 1.0) * c;
}
)";