float4x4    g_stWorldMatrix;
float4x4    g_stViewMatrix;
float4x4    g_stProjectionMatrix;


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
};

STOutput    vs_main(STInput a_stInput)
{
    STOutput stOutput = (STOutput)0;
    float4 stWorldPosition = mul(float4(a_stInput.m_stPosition,1.0f),g_stWorldMatrix);
    float4 stViewPosition = mul(stWorldPosition,g_stViewMatrix);
    stOutput.m_stPosition = mul(stViewPosition,g_stProjectionMatrix);

    return stOutput;
}


float4      ps_main(STOutput a_stInput):COLOR0
{
    float4  stFinalColor = float4(1.0f,1.0f,1.0f,1.0f);
    return stFinalColor;
}


technique outline 
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_main();
        PixelShader = compile ps_3_0 ps_main();
    }
}