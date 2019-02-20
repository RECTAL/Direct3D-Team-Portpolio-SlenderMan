float4x4 g_stWorldMatrix;
float4x4 g_stViewMatrix;
float4x4 g_stProjectionMatrix;

texture g_pTexture;
texture g_pNoiseTexture;


struct VSInput
{
    float3 m_stPosition : POSITION;
    float2 m_stUV       : TEXCOORD;
    float3 m_stNormal   : NORMAL;
    float3 m_stBinormal : BINORMAL;
    float3 m_stTangnet  : TANGENT;
};

struct VSOutput
{
    float4 m_stPosition : POSITION;
    float2 m_stUV       : TEXCOORD0;
};

struct PSInput
{
    float2 m_stUV       : TEXCOORD0;
};

VSOutput vs_main(VSInput a_stInput)
{
    VSOutput stOutput = (VSOutput)0;
    stOutput.m_stPosition = mul(float4(a_stInput.m_stPosition,1.0f),g_stWorldMatrix);
    stOutput.m_stUV = a_stInput.m_stUV;

    return stOutput;
}

sampler2D g_pSampler = sampler_state
{
    Texture = g_pTexture;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler2D g_pNoiseSampler = sampler_state
{
	Texture = g_pNoiseTexture;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

float4 ps_main(PSInput a_stInput):COLOR0
{
    float4 NoiseColor  = tex2D(g_pNoiseSampler,a_stInput.m_stUV);
    float4 baseColor = tex2D(g_pSampler,a_stInput.m_stUV);
   
	float4 stFinalColor;
	stFinalColor = float4(NoiseColor.r*baseColor.r, NoiseColor.g*baseColor.g, NoiseColor.b*baseColor.b, 1.0f);
    return stFinalColor;
}

technique NoiseTexture
{
    pass P0
    {
		CULLMODE = NONE;
        vertexshader = compile vs_3_0 vs_main();
        pixelshader = compile ps_3_0 ps_main();
    }
}