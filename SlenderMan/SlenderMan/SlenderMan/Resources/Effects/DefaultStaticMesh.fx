float4x4 g_stWorldMatrix;
float4x4 g_stViewMatrix;
float4x4 g_stProjectionMatrix;

float4  g_stLightDirection;
float4  g_stViewPosition;

texture g_pTexture;

struct VSInput
{
    float3 m_stPosition :POSITION;
    float2 m_stUV       :TEXCOORD;
    float3 m_stNormal   :NORMAL;
    float3 m_stBinormal :BINORMAL;
    float3 m_stTangent  :TANGENT;
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

VSOutput vs_main(VSInput a_stInput)
{
    VSOutput stOutput = (VSOutput)0;
    float4 stWorldPosition = mul(float4(a_stInput.m_stPosition,1.0f),g_stWorldMatrix);
    stOutput.m_stPosition =  mul(stWorldPosition,g_stViewMatrix);
    stOutput.m_stPosition = mul(stOutput.m_stPosition ,g_stProjectionMatrix);

    float3 stNormal = mul(a_stInput.m_stNormal,g_stWorldMatrix);
    stOutput.m_stNormal = normalize(stNormal);

    float3 stBinormal = mul(a_stInput.m_stBinormal,g_stWorldMatrix);
    stOutput.m_stBinormal = normalize(stBinormal);

    float3 stTangent = mul(a_stInput.m_stTangent,g_stWorldMatrix);
    stOutput.m_stTangent = normalize(stTangent);

    stOutput.m_stLightDirection = normalize(g_stLightDirection.xyz);

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
    Textrue = g_pTexture;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};


float4  ps_main(PSInput a_stInput):COLOR0
{
    float3 stNormal             = normalize(a_stInput.m_stNormal);
    float3 stBinormal           = normalize(a_stInput.m_stBinormal);
    float3 stTangent            = normalize(a_stInput.m_stTangent);
    float3 stLightDirection     = normalize(a_stInput.m_stLightDirection);
    float3 stViewDirection      = normalize(a_stInput.m_stViewDirection);

    float  fDiffuse = saturate(dot(stNormal,stLightDirection));
    float  fSpecular = saturate(dot(reflect(stLightDirection,stNormal),stViewDirection));
    fSpecular = pow(fSpecular,20.0f);

    float4 fBaseColor = tex2D(g_pSampler,a_stInput.m_stUV);
    float4 fDiffuseColor = float4(fBaseColor.rgb*fDiffuse,1.0f);
    float4 fSpecularColor = float4(fBaseColor.rgb*fSpecular,1.0f);
    float4 fAmbient = float4(fBaseColor.rgb*0.2f,1.0f);

    float4 finalColor;

    finalColor = fDiffuseColor+fAmbient+fSpecularColor;

    return finalColor;
}

technique DefaultStaticMesh
{
    pass P0
    {
        vertexshader = compile vs_3_0 vs_main();
        pixelshader = compile ps_3_0 ps_main();
    }
}