float4x4 g_stWorldMatrix;
float4x4 g_stViewMatrix;
float4x4 g_stProjectionMatrix;

float4  g_stLightDirection;
float4  g_stViewPosition;

float4  g_stColor;

struct STInput
{
    float3 m_stPosition : POSITION;
    float3 m_stNormal   : NORMAL;
};

struct STOutput
{
    float4 m_stPosition : POSITION;
    float3 m_stNormal   : TEXCOORD0;
    float3 m_stLightDirection:TEXCOORD1;
    float3 m_stViewDirection:TEXCOORD2;
};


STOutput vs_main(STInput a_stInput)
{
    STOutput stOutput = (STOutput)0;
    float4 stWorldPosition = mul(float4(a_stInput.m_stPosition,1.0f),g_stWorldMatrix);
    stOutput.m_stPosition = mul(stWorldPosition , g_stViewMatrix);
    stOutput.m_stPosition = mul(stOutput.m_stPosition , g_stProjectionMatrix);
   
   
    float3 stNormal = mul(a_stInput.m_stNormal,g_stWorldMatrix);
    stOutput.m_stNormal = normalize(stNormal);

    float3 stLightDirection = g_stLightDirection.xyz;
    stOutput.m_stLightDirection = normalize(stLightDirection);

    float3 stViewPosition  = g_stViewPosition.xyz - stWorldPosition.xyz;
    stOutput.m_stViewDirection = normalize(stViewPosition);

    return stOutput;
}


float4 ps_main(STOutput a_stInput):COLOR
{
    float3 stNormal = normalize(a_stInput.m_stNormal);
    float3 stLightDirection = normalize(a_stInput.m_stLightDirection);
    float3 stViewDirection =normalize(a_stInput.m_stViewDirection);


    float4 baseColor = g_stColor;
    float  fDiffuse = saturate(dot(stLightDirection,stNormal));
    float  fSpecular= saturate(dot(reflect(stLightDirection,stNormal),stViewDirection));

    float4 DiffuseColor = float4(baseColor.rgb*fDiffuse,1.0f);
    float4 SpecularColor = float4(baseColor.rgb*fSpecular,1.0f);
    float4 AmbientColor = float4(baseColor.rgb*0.2f,1.0f);
   
    return DiffuseColor+AmbientColor+SpecularColor;
}

technique DefaultColor
{
    pass P0
    {
        vertexshader = compile vs_3_0 vs_main();
        pixelshader = compile ps_3_0 ps_main();
    }
}
