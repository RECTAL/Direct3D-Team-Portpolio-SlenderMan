float4x4 g_stWorldMatrices[4];
float4x4 g_stViewMatirx;
float4x4 g_stProjectionMatrix;

float4  g_stLightDirection;
float4  g_stViewPosition;

int g_nNumBlends;

texture g_pTexture;


struct VSInput
{
    float3 m_stPosition :POSITION;
    float2 m_stUV       :TEXCOORD;
    float3 m_stNormal   :NORMAL;
    float3 m_stBinormal :BINORMAL;
    float3 m_stTangent  :TANGENT;
    float4 m_stWeight   :BLENDWEIGHT;
};

struct VSOutput
{
    float4 m_stPosition         :POSITION;
    float2 m_stUV               :TEXCOORD0;
    float3 m_stNormal           :TEXCOORD1;
    float3 m_stBinormal         :TEXCOORD2;
    float3 m_stTangent          :TEXCOORD3;
    float3 m_stLightDirection   :TEXCOORD4;
    float3 m_stViewDirection    :TEXCOORD5;
};


VSOutput vs_main(VS_Input a_stInput)
{
    VSOutput stOutput = (VSOutput)0;
    float fLeftWeight = 1.0f;
    for(int i=1;i<=g_nNumBlends;++i)
    {
        float fWeight = a_stInput.m_stWeight[i];
        float4x4 stWorldMatrix = g_stWorldMatrices[i];

        fLeftWeight -= fWeight;
        stOutput.m_stPosition +=mul(float4(a_stInput.m_stPosition,1.0f),stWorldMatrix)*fWeight;
        stOutput.m_stNormal += mul(a_stInput.m_stNormal,(float3x3)stWorldMatrix)*fWeight;
        stOutput.m_stBinormal += mul(a_stInput.m_stBinormal,(float3x3)stWorldMatrix)*fWeight;
        stOutput.m_stTangent += mul(a_stInput.m_stTangent,(float3x3)stWorldMatrix)*fWeight;
    }

    stOutput.m_stPosition += mul(float4(a_stInput.m_stPosition,1.0f),g_stWorldMatrices[0])*fLeftWeight;
    stOutput.m_stNormal += mul(a_stInput.m_stNormal,(float3x3)g_stWorldMatrices[0])*fLeftWeight;
    stOutput.m_stBinormal += mul(a_stInput.m_stBinormal,(float3x3)g_stWorldMatrices[0])*fLeftWeight;
    stOutput.m_stTangent  += mul(a_stInput.m_stTangent,(float3x3)g_stWorldMatrices[0])*fLeftWeight;

    stOutput.m_stPosition.w = 1.0f;
    float4 stWorldPosition = stOutput.m_stPosition;

    stOutput.m_stPosition = mul(stWorldPosition,g_stViewMatirx);
    stOutput.m_stPosition = mul(stWorldPosition,g_stProjectionMatrix);

    float3 stLightDirection = g_stLightDirection.xyz;
    stOutput.m_stLightDirection = normalize(stLightDirection);

    float3 stViewDirection = g_stViewPosition.xyz - stWorldPosition.xyz;
    stOutput.m_stViewDirection = normalize(stViewDirection);

    stOutput.m_stUV = a_stInput.m_stUV;

    return stOutput;
}

struct PSInput
{
    float2 m_stUV               :TEXCOORD0;
    float3 m_stNormal           :TEXCOORD1;
    float3 m_stBinormal         :TEXCOORD2;
    float3 m_stTangent          :TEXCOORD3;
    float3 m_stLightDirection   :TEXCOORD4;
    float3 m_stViewDirection    :TEXCOORD5;
};

sampler2D g_pSampler = sampler_state
{
    Texture = g_pTexture;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter =  Linear;
};


float4 ps_main(PSInput a_stInput):COLOR0
{
    float3 stNormal         = normalize(a_stInput.m_stNormal);
    float3 stBinormal       = normalize(a_stInput.m_stBinormal);
    float3 stTangent        = normalize(a_stInput.m_stTangent);
    float3 stLightDirection = normalize(a_stInput.m_stLightDirection);
    float3 stViewDirection  = normalize(a_stInput.m_stViewDirection);

    float fDiffuse  = saturate(dot(stLightDirection,stNormal));
    float fSpecular = saturate(dot(reflect(stLightDirection,stNormal),stViewDirection));
    fSpecular = pow(fSpecular,20.0f);

    float4 fBaseColor = tex2D(g_pSampler,a_stInput.m_stUV);
    float4 fDiffuseColor =  float4(fBaseColor.rgb*fDiffuse,1.0f);
    float4 fSpecularColor = float4(fBaseColor.rgb*fSpecular,1.0f);
    float4 fAmbient =  float4(fAmbient.xyz*0.2f,1.0f);


    float4 finalColor;
    finalColor = fDiffuseColor+fSpecularColor+fAmbient;

    return finalColor;
}

technique DefaultSkinnedMesh
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_main();
        PixelShader = compile ps_3_0 ps_main();
    }
}