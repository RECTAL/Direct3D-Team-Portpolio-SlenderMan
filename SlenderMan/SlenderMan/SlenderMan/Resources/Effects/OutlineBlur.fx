float4x4    g_stWorldMatrix;
float4x4    g_stViewMatrix;
float4x4    g_stProjectionMatrix;

float4      g_stOutlineColor;
float       g_fSize;


texture g_pOutlineRenderTexture;
texture g_pRenderTexture;

float3x3 g_stHMatrix = {
	-1.0f, 0.0f, 1.0f,
	-2.0f, 0.0f, 2.0f,
	-1.0f, 0.0f, 1.0f
};

float3x3 g_stVMatrix = {
	 1.0f,  2.0f,  1.0f,
	 0.0f,  0.0f,  0.0f,
	-1.0f, -2.0f, -1.0f
};

struct STInput
{
    float3  m_stPosition :POSITION;
    float2  m_stUV       :TEXCOORD;
    float3  m_stNormal   :NORMAL;
    float3  m_stBinormal :BINORMAL;
    float3  m_stTangent  :TANGENT;
};

struct STOutput
{
    float4 m_stPosition :POSITION;
    float2  m_stUV       :TEXCOORD;
};

STOutput    vs_main(STInput a_stInput)
{
	STOutput stOutput = (STOutput)0;
	stOutput.m_stPosition = mul(float4(a_stInput.m_stPosition, 1.0f), g_stWorldMatrix);
	stOutput.m_stUV = a_stInput.m_stUV;

    return stOutput;
}


sampler2D g_pOutlineRenderSampler = sampler_state
{
    Texture = g_pOutlineRenderTexture;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler2D g_pRenderSampler = sampler_state
{
    Texture = g_pRenderTexture;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};


float4     ps_main(STOutput a_stInput):COLOR0
{
    float fHSumValue = 0.0f;
    float fVSumValue = 0.0f;

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            float2 stOffset = float2(i-1,j-1)*(1.0f/g_fSize);
            float3 stColor = tex2D(g_pOutlineRenderSampler,a_stInput.m_stUV + stOffset).rgb;
            float fValue = dot(stColor,float3(0.3f,0.59f,0.12f));

            fHSumValue += fValue * g_stHMatrix[i][j];
			fVSumValue += fValue * g_stVMatrix[i][j];
        }
    }

    float fFinalValue = sqrt(pow(fHSumValue, 2.0f) + pow(fVSumValue, 2.0f));
    float4 stOutlineColor = g_stOutlineColor*fFinalValue;
    float4 stRenderColor = tex2D(g_pRenderSampler,a_stInput.m_stUV);

    float4 stFinalColor;

   
	if(stOutlineColor.a <= 0.0001f) stFinalColor = stRenderColor;
	else stFinalColor = float4(stOutlineColor.rgb,1.0f);
	//stFinalColor =  stOutlineColor + stRenderColor;

    return stFinalColor;
}


technique outlineBlur
{
    pass P0
    {
		CULLMODE = NONE;
        VertexShader = compile vs_3_0 vs_main();
        PixelShader = compile ps_3_0 ps_main();
    }
}