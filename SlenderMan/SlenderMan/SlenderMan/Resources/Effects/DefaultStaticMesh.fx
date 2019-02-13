float4x4 g_stWorldMatrix;
float4x4 g_stViewMatrix;
float4x4 g_stProjectionMatrix;

float4  g_stLightDirection;
float4  g_stViewPosition;

float   g_fFogDensity;
float   g_fFogEnd;


int	nNumSpotLight;
float4	g_stSpotLightPosition[10];
float4	g_stSpotLightForward[10];
float	g_fTheta[10];
float	g_fPhi[10];
float	g_fSpotDistance[10];

int nNumPointLight;
float4	g_stPointLightPosition[10];
float	g_fPointDistance[10];

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
    float3 m_stWorldPosition    :TEXCOORD6;
};

VSOutput vs_main(VSInput a_stInput)
{
    VSOutput stOutput = (VSOutput)0;
    float4 stWorldPosition = mul(float4(a_stInput.m_stPosition,1.0f),g_stWorldMatrix);
    stOutput.m_stPosition =  mul(stWorldPosition,g_stViewMatrix);
    stOutput.m_stPosition = mul(stOutput.m_stPosition ,g_stProjectionMatrix);

    float3 stNormal = mul(a_stInput.m_stNormal,(float3x3)g_stWorldMatrix);
    stOutput.m_stNormal = normalize(stNormal);

    float3 stBinormal = mul(a_stInput.m_stBinormal,(float3x3)g_stWorldMatrix);
    stOutput.m_stBinormal = normalize(stBinormal);

    float3 stTangent = mul(a_stInput.m_stTangent,(float3x3)g_stWorldMatrix);
    stOutput.m_stTangent = normalize(stTangent);

    stOutput.m_stLightDirection = normalize(g_stLightDirection.xyz);

    float3 stViewDirection = g_stViewPosition.xyz - stWorldPosition.xyz;
    stOutput.m_stViewDirection = normalize(stViewDirection);

    stOutput.m_stWorldPosition = stWorldPosition;

    stOutput.m_stUV = a_stInput.m_stUV;

    return stOutput;
}


struct VSOutput_Fog
{
    float4 m_stPosition         :POSITION;
    float2 m_stUV               :TEXCOORD0;
    float3 m_stNormal           :TEXCOORD1;
    float3 m_stBinormal         :TEXCOORD2;
    float3 m_stTangent          :TEXCOORD3;
    float3 m_stLightDirection   :TEXCOORD4;
    float3 m_stViewDirection    :TEXCOORD5;
    float3 m_stWorldPosition    :TEXCOORD6;
    float  m_fFogValue          :TEXCOORD7;
};


VSOutput_Fog vs_FogMain(VSInput a_stInput)
{
    VSOutput_Fog stOutput = (VSOutput_Fog)0;
    float4 stWorldPosition = mul(float4(a_stInput.m_stPosition,1.0f),g_stWorldMatrix);
    float4 stViewPosition = mul(stWorldPosition, g_stViewMatrix);
    stOutput.m_stPosition = mul(stViewPosition,g_stProjectionMatrix);

    float3 stNormal = mul(a_stInput.m_stNormal,(float3x3)g_stWorldMatrix);
    stOutput.m_stNormal = normalize(stNormal);

    float3 stBinormal = mul(a_stInput.m_stBinormal,(float3x3)g_stWorldMatrix);
    stOutput.m_stBinormal = normalize(stBinormal);

    float3 stTangent = mul(a_stInput.m_stTangent,(float3x3)g_stWorldMatrix);
    stOutput.m_stTangent = normalize(stTangent);

    stOutput.m_stLightDirection = normalize(g_stLightDirection.xyz);

    float3 stViewDirection = g_stViewPosition.xyz - stWorldPosition.xyz;
    stOutput.m_stViewDirection = normalize(stViewDirection);

    stOutput.m_stWorldPosition = stWorldPosition;

    float fDistence = stViewPosition.z/g_fFogEnd*4;
    float f = exp(-fDistence * g_fFogDensity);
    stOutput.m_fFogValue = f;

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
    float3 m_stWorldPosition    :TEXCOORD6;
};


struct PSOutput_Fog
{
    float2 m_stUV               :TEXCOORD0;
    float3 m_stNormal           :TEXCOORD1;
    float3 m_stBinormal         :TEXCOORD2;
    float3 m_stTangent          :TEXCOORD3;
    float3 m_stLightDirection   :TEXCOORD4;
    float3 m_stViewDirection    :TEXCOORD5;
    float3 m_stWorldPosition    :TEXCOORD6;
    float  m_fFogValue          :TEXCOORD7;
};

sampler2D g_pSampler = sampler_state
{
    Texture = g_pTexture;
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

    float4 fSpotLightColor = float4(0.0f,0.0f,0.0f,1.0f);

	

    for(int i=0;i<nNumSpotLight;i++)
    {
        float3 stSpotPosition = g_stSpotLightPosition[i].xyz;
        float3 stSpotDirection = g_stSpotLightForward[i].xyz;
        float fTheta = g_fTheta[i];
        float fPhi = g_fPhi[i];
        float fSpotDistance = g_fSpotDistance[i];
        float4 stSpotColor = fBaseColor;

        float3 stSpotToPos = normalize(a_stInput.m_stWorldPosition-stSpotPosition);
        float fDistance = distance(a_stInput.m_stWorldPosition,stSpotPosition);

		//if (fDistance > fSpotDistance)continue;

        float delDegree = acos(dot(stSpotDirection,stSpotToPos));

        if(delDegree < fTheta) 
        {
			stSpotColor.rgb *= saturate(dot(stSpotDirection, stSpotToPos));
			stSpotColor.rgb = lerp(stSpotColor.rgb, fBaseColor.rgb*1.2f, (fTheta - delDegree) / fTheta);
        }
        if(delDegree<fPhi)
        {
            stSpotColor.rgb *= saturate(dot(stSpotDirection,stSpotToPos));
            stSpotColor.rgb = lerp(float3(0.0f, 0.0f, 0.0f),stSpotColor.rgb,(fPhi - delDegree)/fPhi);
        } 
        else continue;

		fSpotLightColor.rgb += lerp(float3(0.0f, 0.0f, 0.0f),stSpotColor.rgb, min(fSpotDistance /fDistance,2.0f));
    }


	float4 finalColor;

	finalColor = fDiffuseColor + fAmbient + fSpecularColor + fSpotLightColor;

    return finalColor;
}

float4  ps_FogMain(PSOutput_Fog a_stInput):COLOR0
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
    float4 fFogColor = float4(0.0f,0.0f,0.0f,1.0f);
    float4 fSpotLightColor = float4(0.0f,0.0f,0.0f,1.0f);

	

    for(int i=0;i<nNumSpotLight;i++)
    {
        float3 stSpotPosition = g_stSpotLightPosition[i].xyz;
        float3 stSpotDirection = g_stSpotLightForward[i].xyz;
        float fTheta = g_fTheta[i];
        float fPhi = g_fPhi[i];
        float fSpotDistance = g_fSpotDistance[i];
        float4 stSpotColor = fBaseColor;

        float3 stSpotToPos = normalize(a_stInput.m_stWorldPosition-stSpotPosition);
        float fDistance = distance(a_stInput.m_stWorldPosition,stSpotPosition);

		//if (fDistance > fSpotDistance)continue;

        float delDegree = acos(dot(stSpotDirection,stSpotToPos));

        if(delDegree < fTheta) 
        {
			stSpotColor.rgb *= saturate(dot(stSpotDirection, stSpotToPos));
			stSpotColor.rgb = lerp(stSpotColor.rgb, fBaseColor.rgb*1.2f, (fTheta - delDegree) / fTheta);
        }
        if(delDegree<fPhi)
        {
            stSpotColor.rgb *= saturate(dot(stSpotDirection,stSpotToPos));
            stSpotColor.rgb = lerp(float3(0.0f, 0.0f, 0.0f),stSpotColor.rgb,(fPhi - delDegree)/fPhi);
        } 
        else continue;

		fSpotLightColor.rgb += lerp(float3(0.0f, 0.0f, 0.0f),stSpotColor.rgb, min(fSpotDistance /fDistance,2.0f));
    }


	float4 finalColor;

	finalColor = fDiffuseColor + fAmbient + fSpecularColor + fSpotLightColor;
    finalColor = a_stInput.m_fFogValue*finalColor +(1-a_stInput.m_fFogValue)*fFogColor;

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

technique FogStaticMesh
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_FogMain();
        PixelShader = compile ps_3_0 ps_FogMain();
    }
}