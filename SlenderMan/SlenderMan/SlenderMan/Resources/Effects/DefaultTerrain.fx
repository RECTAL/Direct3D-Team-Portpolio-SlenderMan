
float4x4 g_stWorldMatrix;
float4x4 g_stViewMatrix;
float4x4 g_stProjectionMatrix;


int	nNumSpotLight;
float4	g_stSpotLightPosition[10];
float4	g_stSpotLightForward[10];
float	g_fTheta[10];
float	g_fPhi[10];
float	g_fSpotDistance[10];

int nNumPointLight;
float4	g_stPointLightPosition[10];
float	g_fPointDistance[10];



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
	float3 m_stWorldPosition    :TEXCOORD4;
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

	stOutput.m_stWorldPosition = stWorldPosition.xyz;

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
	float2 stUV = a_stInput.m_stUV;
	stUV.x = stUV.x / 80;
	stUV.y = stUV.y / 80;

	float4 stColorA = tex2D(g_pSamplerA, a_stInput.m_stUV);
	float4 stColorB = tex2D(g_pSamplerB, a_stInput.m_stUV);
	float4 stColorC = tex2D(g_pSamplerC, a_stInput.m_stUV);
	float4 stColorD = tex2D(g_pSamplerD, a_stInput.m_stUV);
	float4 stSplatColor = tex2D(g_pSplatSampler, stUV);

	float fBlackPercent = 1.0f - saturate(stSplatColor.r + stSplatColor.g + stSplatColor.b);

	//float4 stDiffuseColor = (stColorA * stSplatColor.r) +
	//	(stColorB * stSplatColor.g) +
	//	(stColorC * stSplatColor.b) +
	//	(stColorD * fBlackPercent);

	float4 stDiffuseColor = float4(0.4f, 0.4f, 0.4f, 1.0f);

	float4 fSpotLightColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	for (int i = 0; i < nNumSpotLight; i++)
	{
		float3 stSpotPosition = g_stSpotLightPosition[i].xyz;
		float3 stSpotDirection = g_stSpotLightForward[i].xyz;
		float fTheta = g_fTheta[i];
		float fPhi = g_fPhi[i];
		float fSpotDistance = g_fSpotDistance[i];
		float4 stSpotColor = stDiffuseColor;

		float3 stSpotToPos = normalize(a_stInput.m_stWorldPosition - stSpotPosition);
		float fDistance = distance(a_stInput.m_stWorldPosition, stSpotPosition);

		//if (fDistance > fSpotDistance)continue;

		float delDegree = acos(dot(stSpotDirection, stSpotToPos));

		if (delDegree < fTheta)
		{
			stSpotColor.rgb *= saturate(dot(stSpotDirection, stSpotToPos));
			stSpotColor.rgb = lerp(stSpotColor.rgb, stDiffuseColor.rgb*1.2f, (fTheta - delDegree) / fTheta);
		}
		if (delDegree < fPhi)
		{
			stSpotColor.rgb *= saturate(dot(stSpotDirection, stSpotToPos));
			stSpotColor.rgb = lerp(float3(0.0f, 0.0f, 0.0f), stSpotColor.rgb, (fPhi - delDegree) / fPhi);
		}
		else continue;

		fSpotLightColor.rgb += lerp(float3(0.0f, 0.0f, 0.0f), stSpotColor.rgb,  min(fSpotDistance /fDistance,5.0f));
	}


	float4 stFinalColor = stDiffuseColor+ fSpotLightColor;
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
