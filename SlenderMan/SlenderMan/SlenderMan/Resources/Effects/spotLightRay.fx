float4x4    g_stWorldMatrix;
float4x4    g_stViewMatrix;
float4x4    g_stProjectionMatrix;

float4      g_stViewPosition;

float g_fFresnel;							//1.0f
float g_fAlphaOffset;						//1,0f
float g_fNoiseSpeed;						//0.5f
float g_fAmbient;							//0.3f
float g_fIntensity;//0.2f
float g_fFade;//1.0f
float g_fWind;//0.1f
float g_fTime;

float mod512(float x) { return x - floor(x * (1.0 / 512.0)) * 512.0; }
float4 mod512(float4 x) { return x - floor(x * (1.0 / 512.0)) * 512.0; }
float4 perm(float4 x) { return mod512(((x * 34.0) + 1.0) * x); }

float noise(float3 p) {
	float3 a = floor(p);
	float3 d = p - a;
	d = d * d * (3.0 - 2.0 * d);

	float4 b = a.xxyy + float4(0.0, 1.0, 0.0, 1.0);
	float4 k1 = perm(b.xyxy);
	float4 k2 = perm(k1.xyxy + b.zzww);

	float4 c = k2 + a.zzzz;
	float4 k3 = perm(c);
	float4 k4 = perm(c + 1.0);

	float4 o1 = frac(k3 * (1.0 / 41.0));
	float4 o2 = frac(k4 * (1.0 / 41.0));

	float4 o3 = o2 * d.z + o1 * (1.0 - d.z);
	float2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

	return o4.y * d.y + o4.x * (1.0 - d.y);
}


struct STInput
{
    float3 m_stPosition     :POSITION;
    float2 m_stUV           :TEXCOORD;
    float3 m_stNormal       :NORMAL;
    float3 m_stBinormal     :BINORMAL;
    float3 m_stTangent      :TANGENT;
};


struct STOutput
{
    float4 m_stPosition     :POSITION;
    float2 m_stUV           :TEXCOORD0;
    float3 m_stNormal       :TEXCOORD1;
    float3 m_stWorldPos     :TEXCOORD2;
};

STOutput    vs_main(STInput a_stInput)
{
    STOutput stOutput = (STOutput)0;
	float Noise = g_fWind* noise(a_stInput.m_stNormal + float3(g_fTime, g_fTime, g_fTime));
	float4 newPosition = float4(a_stInput.m_stPosition.xyz+ Noise *a_stInput.m_stNormal,1);

    float4 stWorldPosition = mul(float4(a_stInput.m_stPosition,1.0f),g_stWorldMatrix);
    float4 stViewPosition = mul(stWorldPosition,g_stViewMatrix);
    stOutput.m_stPosition = mul(stViewPosition,g_stProjectionMatrix);
    stOutput.m_stWorldPos = stWorldPosition.xyz;

    float3 stNormal = mul(a_stInput.m_stNormal,g_stWorldMatrix);
    stOutput.m_stNormal = normalize(stNormal);

    stOutput.m_stUV = a_stInput.m_stUV;

    return stOutput;
}




float4  ps_main(STOutput a_stInput):COLOR0
{
    float3 stNormal = normalize(a_stInput.m_stNormal);
	float3 stViewDirection = g_stViewPosition.xyz - a_stInput.m_stWorldPos.xyz;
    stViewDirection = normalize(stViewDirection);

	float nu = (a_stInput.m_stUV.x < 0.5f) ? a_stInput.m_stUV.x : 1.0f - a_stInput.m_stUV.x;
	nu = pow(abs(nu), 2.0f);

	float2 newUV = float2(nu, a_stInput.m_stUV.y);

	float n_a = noise(float3(newUV*5.0f, 1.0f) + float3(g_fTime, g_fTime, g_fTime)*g_fNoiseSpeed*-1.0f)*g_fIntensity + g_fAmbient;
	float n_b = noise(float3(newUV*10.0f, 1.0f) + float3(g_fTime, g_fTime, g_fTime)*g_fNoiseSpeed*-1.0f)*0.9f;
	float n_c = noise(float3(newUV*20.0f, 1.0f) + float3(g_fTime, g_fTime, g_fTime)*g_fNoiseSpeed*-2.0f)*0.9f;
	float n_d = pow(abs(noise(float3(newUV*30.0f, 1.0f) + float3(g_fTime, g_fTime, g_fTime)*g_fNoiseSpeed*-2.0)), 2.0f)*0.9f;
	float rNoise = n_a + n_b + n_c + n_d;
	rNoise = (rNoise < 0.0f) ? 0.0f : rNoise;
	float4 color = float4(rNoise, rNoise, rNoise, 1.0f);;


    float raycast =saturate(dot(stViewDirection,stNormal));
    float fresnel = pow(raycast,g_fFresnel);

    float fade = saturate(pow(abs(1.0f - a_stInput.m_stUV.y),g_fFade));
	color.a *= fresnel*g_fAlphaOffset*fade;

	return float4(color.r, color.g, color.b, color.a);
	//return float4(color.r*color.a, color.g*color.a, color.b*color.a*0.2f, color.a);
}


technique spotLightRay
{
    pass P0
    {
        VertexShader = compile vs_3_0 vs_main();
        PixelShader = compile ps_3_0 ps_main();
    }
}