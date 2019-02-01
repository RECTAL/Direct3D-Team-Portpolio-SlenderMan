
float4x4 g_stWorldMatrix;
float4x4 g_stViewMatrix;
float4x4 g_stProjectionMatrix;


texture g_pTextureA;
texture g_pTextureB;
texture g_pTextureC;
texture g_pTextureD;
texture g_pSplatTexture;


struct STInput
{
	float3 m_stPosition : POSITION;
	float2 m_stUV 		: TEXCOORD;
	float3 m_stNormal 	: NORMAL;
	float3 m_stBinormal : BINORMAL;
	float3 m_stTangent 	: TANGENT;
};


struct STOutput
{
	float4 m_stPosition : POSITION;
	float2 m_stUV 		: TEXCOORD0;
	float3 m_stNormal 	: TEXCOORD1;
	float3 m_stBinormal : TEXCOORD2;
	float3 m_stTangent 	: TEXCOORD3;
};

STOutput vs_main(STInput a_stInput)
{
	STOutput stOutput = (STOutput)0;
	float4 stWorldPosition = mul(float4(a_stInput.m_stPosition, 1.0f), g_stWorldMatrix);

	stOutput.m_stPosition = mul(stWorldPosition, g_stViewMatrix);
	stOutput.m_stPosition = mul(stOutput.m_stPosition, g_stProjectionMatrix);

	float3 stNormal = mul(a_stInput.m_stNormal, (float3x3)g_stWorldMatrix);
	stOutput.m_stNormal = normalize(stNormal);

	float3 stBinormal = mul(a_stInput.m_stBinormal, (float3x3)g_stWorldMatrix);
	stOutput.m_stBinormal = normalize(stBinormal);

	float3 stTangent = mul(a_stInput.m_stTangent, (float3x3)g_stWorldMatrix);
	stOutput.m_stTangent = normalize(stTangent);

	stOutput.m_stUV = a_stInput.m_stUV;
	return stOutput;
}


sampler2D g_pSamplerA = sampler_state
{
	Texture = g_pTextureA;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

sampler2D g_pSamplerB = sampler_state
{
	Texture = g_pTextureB;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

sampler2D g_pSamplerC = sampler_state
{
	Texture = g_pTextureC;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

sampler2D g_pSamplerD = sampler_state
{
	Texture = g_pTextureD;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

sampler2D g_pSplatSampler = sampler_state
{
	Texture = g_pSplatTexture;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

float4 ps_main(STOutput a_stInput) : COLOR0
{
	float4 stColorA = tex2D(g_pSamplerA, a_stInput.m_stUV);
	float4 stColorB = tex2D(g_pSamplerB, a_stInput.m_stUV);
	float4 stColorC = tex2D(g_pSamplerC, a_stInput.m_stUV);
	float4 stColorD = tex2D(g_pSamplerD, a_stInput.m_stUV);
	float4 stSplatColor = tex2D(g_pSplatSampler, a_stInput.m_stUV);

	float fBlackPercent = 1.0f - saturate(stSplatColor.r + stSplatColor.g + stSplatColor.b);

	float4 stDiffuseColor = (stColorA * stSplatColor.r) +
		(stColorB * stSplatColor.g) +
		(stColorC * stSplatColor.b) +
		(stColorD * fBlackPercent);


	float4 stFinalColor = stDiffuseColor;
	return stFinalColor;
}

technique DefaultTerrain
{
	pass P0
	{
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}
